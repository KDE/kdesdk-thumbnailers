/*
    SPDX-FileCopyrightText: 2011-2012 Ni Hui <shuizhuyuanluo@126.com>

    SPDX-License-Identifier: GPL-2.0-only OR GPL-3.0-only OR LicenseRef-KDE-Accepted-GPL
*/

#ifndef POCREATOR_H
#define POCREATOR_H

#include <KIO/ThumbnailCreator>

class PoCreator : public KIO::ThumbnailCreator
{
    public:
        PoCreator(QObject *parent, const QVariantList &args);
        ~PoCreator() override;
        KIO::ThumbnailResult create( const KIO::ThumbnailRequest &request ) override;
};

#endif // POCREATOR_H
