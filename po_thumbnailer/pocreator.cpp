/*
 *  This file is part of kde-thumbnailer-po
 *  Copyright (C) 2011-2012 Ni Hui <shuizhuyuanluo@126.com>
 *
 *  This program is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU General Public License as
 *  published by the Free Software Foundation; either version 2 of
 *  the License or (at your option) version 3 or any later version
 *  accepted by the membership of KDE e.V. (or its successor approved
 *  by the membership of KDE e.V.), which shall act as a proxy
 *  defined in Section 14 of version 3 of the license.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "pocreator.h"

#include <gettext-po.h>
#include <QColor>
#include <QImage>
#include <QLatin1String>
#include <QPainter>
#include <QWidget>
#include <KLocalizedString>

#include "pocreatorsettings.h"
#include "ui_pocreatorform.h"

extern "C" {
    Q_DECL_EXPORT ThumbCreator* new_creator() {
        return new PoCreator;
    }
}

static bool readerror = false;

static void xerror( int severity,
                    po_message_t /*message*/,
                    const char */*filename*/, size_t /*lineno*/, size_t /*column*/,
                    int /*multiline_p*/, const char */*message_text*/ )
{
    if ( severity == PO_SEVERITY_ERROR || severity == PO_SEVERITY_FATAL_ERROR )
        readerror = true;
}

static void xerror2( int severity,
                     po_message_t /*message1*/,
                     const char */*filename1*/, size_t /*lineno1*/, size_t /*column1*/,
                     int /*multiline_p1*/, const char */*message_text1*/,
                     po_message_t /*message2*/,
                     const char */*filename2*/, size_t /*lineno2*/, size_t /*column2*/,
                     int /*multiline_p2*/, const char */*message_text2*/ )
{
    if ( severity == PO_SEVERITY_ERROR || severity == PO_SEVERITY_FATAL_ERROR )
        readerror = true;
}

static bool get_po_info( const char* filepath, int& translate, int& untranslate, int& fuzzy, int& obsolete )
{
    po_file_t pofile;
    const struct po_xerror_handler handler = { xerror, xerror2 };

    pofile = po_file_read( filepath, &handler );
    if ( pofile == NULL || readerror )
        return false;

    po_message_iterator_t it;
    it = po_message_iterator( pofile, NULL );
    po_message_t msg;
    const char* msgstr;
    while ( ( msg = po_next_message( it ) ) != NULL ) {
        if ( po_message_is_obsolete( msg ) )
            ++obsolete;
        else if ( po_message_is_fuzzy( msg ) )
            ++fuzzy;
        else {
            msgstr = po_message_msgstr( msg );
            if ( msgstr[0] == '\0' )
                ++untranslate;
            else
                ++translate;
        }
    }
    po_message_iterator_free( it );

    /// do not count domain header as translated message
    const char* header = po_file_domain_header( pofile, NULL );
    if ( header != NULL )
        --translate;

    po_file_free( pofile );

    return true;
}

PoCreator::PoCreator()
{
}

PoCreator::~PoCreator()
{
}

bool PoCreator::create( const QString& path, int width, int height, QImage& img )
{
    int translate = 0;
    int untranslate = 0;
    int fuzzy = 0;
    int obsolete = 0;

    if ( !get_po_info( path.toLocal8Bit(), translate, untranslate, fuzzy, obsolete ) )
        return false;

    int total = translate + untranslate + fuzzy + obsolete;

    int d = ( width < height ) ? width - 2 : height - 2;

    QImage pix( d + 2, d + 2, QImage::Format_ARGB32_Premultiplied );
    pix.fill( Qt::transparent ); /// transparent background

    int circle = 16 * 360;
    int untranslateAngle = untranslate * circle / total;
    int fuzzyAngle = fuzzy * circle / total;
    int obsoleteAngle = obsolete * circle / total;
    int translateAngle = circle - untranslateAngle - fuzzyAngle - obsoleteAngle;

    QPainter p( &pix );
    p.setRenderHint( QPainter::Antialiasing );

    if ( fuzzyAngle > 0 ) {
        p.setBrush( PoCreatorSettings::self()->fuzzyColor() );
        if ( fuzzy == total )
            p.drawEllipse( 1, 1, d, d );
        else
            p.drawPie( 1, 1, d, d, 0, -fuzzyAngle );
    }
    if ( untranslateAngle > 0 ) {
        p.setBrush( PoCreatorSettings::self()->untranslatedColor() );
        if ( untranslate == total )
            p.drawEllipse( 1, 1, d, d );
        else
            p.drawPie( 1, 1, d, d, -fuzzyAngle, -untranslateAngle );
    }
    if ( obsoleteAngle > 0 ) {
        p.setBrush( PoCreatorSettings::self()->obsoletedColor() );
        if ( obsolete == total )
            p.drawEllipse( 1, 1, d, d );
        else
            p.drawPie( 1, 1, d, d, -fuzzyAngle-untranslateAngle, -obsoleteAngle );
    }
    if ( translateAngle > 0 ) {
        p.setBrush( PoCreatorSettings::self()->translatedColor() );
        if ( translate == total )
            p.drawEllipse( 1, 1, d, d );
        else
            p.drawPie( 1, 1, d, d, -fuzzyAngle-untranslateAngle-obsoleteAngle, -translateAngle );
    }

    img = pix;

    return true;
}

class PoCreatorFormWidget : public QWidget, public Ui::PoCreatorForm
{
public:
    PoCreatorFormWidget() { setupUi( this ); }
};

QWidget* PoCreator::createConfigurationWidget()
{
    PoCreatorFormWidget* cw = new PoCreatorFormWidget;
    cw->translatedButton->setColor( PoCreatorSettings::self()->translatedColor() );
    cw->fuzzyButton->setColor( PoCreatorSettings::self()->fuzzyColor() );
    cw->untranslatedButton->setColor( PoCreatorSettings::self()->untranslatedColor() );
    cw->obsoletedButton->setColor( PoCreatorSettings::self()->obsoletedColor() );
    return cw;
}

void PoCreator::writeConfiguration( const QWidget* configurationWidget )
{
    const PoCreatorFormWidget* cw = static_cast<const PoCreatorFormWidget*>(configurationWidget);
    PoCreatorSettings::self()->setTranslatedColor( cw->translatedButton->color() );
    PoCreatorSettings::self()->setFuzzyColor( cw->fuzzyButton->color() );
    PoCreatorSettings::self()->setUntranslatedColor( cw->untranslatedButton->color() );
    PoCreatorSettings::self()->setObsoletedColor( cw->obsoletedButton->color() );
    PoCreatorSettings::self()->writeConfig();
}
