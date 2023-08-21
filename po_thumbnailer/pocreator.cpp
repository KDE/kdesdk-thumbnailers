/*
    SPDX-FileCopyrightText: 2011-2012 Ni Hui <shuizhuyuanluo@126.com>

    SPDX-License-Identifier: GPL-2.0-only OR GPL-3.0-only OR LicenseRef-KDE-Accepted-GPL
*/

#include "pocreator.h"

#include <gettext-po.h>
#include <QColor>
#include <QImage>
#include <QLatin1String>
#include <QPainter>
#include <QWidget>

#include <KLocalizedString>
#include <KPluginFactory>

#include "pocreatorsettings.h"
#include "ui_pocreatorform.h"

K_PLUGIN_CLASS_WITH_JSON(PoCreator, "pothumbnail.json")

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

PoCreator::PoCreator(QObject *parent, const QVariantList &args)
    : KIO::ThumbnailCreator(parent, args)
{
}

PoCreator::~PoCreator()
{
}

KIO::ThumbnailResult PoCreator::create( const KIO::ThumbnailRequest &request )
{
    int translate = 0;
    int untranslate = 0;
    int fuzzy = 0;
    int obsolete = 0;

    if ( !get_po_info( request.url().toLocalFile().toLocal8Bit().constData(), translate, untranslate, fuzzy, obsolete ) )
        return KIO::ThumbnailResult::fail();

    int total = translate + untranslate + fuzzy + obsolete;

    if (total == 0) {
        // Treat a .po file with no strings as an invalid file
        return KIO::ThumbnailResult::fail();
    }

    int d = ( request.targetSize().width() < request.targetSize().height() ) ? request.targetSize().width() - 2 : request.targetSize().height() - 2;

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

    return KIO::ThumbnailResult::pass(pix);
}

#include "pocreator.moc"
