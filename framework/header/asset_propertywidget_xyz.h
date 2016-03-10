/****************************************************************************
**
** Copyright (C) 2016
**
** This file is generated by the Magus toolkit
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
** OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
** LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
**
****************************************************************************/

#ifndef MAGUS_PROPERTY_WIDGET_XYZ_H
#define MAGUS_PROPERTY_WIDGET_XYZ_H

#include <QWidget>
#include <QCheckBox>
#include <QLineEdit>
#include "asset_propertywidget.h"

QT_BEGIN_NAMESPACE

QT_END_NAMESPACE

namespace Magus
{
    /****************************************************************************
    Property class to fill in X, Y and Z values
    ***************************************************************************/
    class QtXYZProperty : public QtProperty
    {
        Q_OBJECT

        public:
            QtXYZProperty(const QString& title, qreal x = 0.0, qreal y = 0.0, qreal z = 0.0, QWidget* parent = 0);
            virtual ~QtXYZProperty(void) {}
            void setXYZ (qreal x, qreal y, qreal z);
            void setX (qreal x);
            void setY (qreal y);
            void setZ (qreal z);
            const qreal getX (void) const;
            const qreal getY (void) const;
            const qreal getZ (void) const;
            void enableX (bool enabled);
            void enableY (bool enabled);
            void enableZ (bool enabled);

        private slots:
            void propertyValueChanged(void);

        private:
            QLineEdit* mXEdit;
            QLineEdit* mYEdit;
            QLineEdit* mZEdit;
    };
}

#endif