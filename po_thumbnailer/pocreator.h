/*
    SPDX-FileCopyrightText: 2011-2012 Ni Hui <shuizhuyuanluo@126.com>

    SPDX-License-Identifier: GPL-2.0-only OR GPL-3.0-only OR LicenseRef-KDE-Accepted-GPL
*/

#ifndef POCREATOR_H
#define POCREATOR_H

#include <KIO/ThumbCreator>

class PoCreator : public ThumbCreator
{
    public:
        PoCreator();
        ~PoCreator() override;
        bool create( const QString& path, int width, int height, QImage& img ) override;
        QWidget* createConfigurationWidget() override;
        void writeConfiguration( const QWidget* configurationWidget ) override;
};

#endif // POCREATOR_H
