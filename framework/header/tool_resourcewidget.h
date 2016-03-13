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

#ifndef MAGUS_RESOURCE_WIDGET_H
#define MAGUS_RESOURCE_WIDGET_H

#include <QTreeWidget>
#include <QMimeData>
#include <QStringList>
#include <QDockWidget>
#include <QMap>
#include "constants.h"
#include "tool_resourcewidget_main.h"

QT_BEGIN_NAMESPACE

QT_END_NAMESPACE

namespace Magus
{
    static const int TOOL_SOURCES_LEVEL_X000_PBS = 1;
    static const int TOOL_SOURCES_LEVEL_X000_UNLIT = 2;

    /****************************************************************************
    The QtResourceWidget is an example of a resource widget for media and other
    resources, used in 3D editors.
    Note:
    This widget can be used as a template for resource management. It can be
    used as is, but its main purpose is to provide a start and extend the widget
    for your own situation.

    IMPORTANT:
    =========
    This class is changed to facilitate the HLMS editor. Do not use it as a
    generic Magus class.
    ***************************************************************************/
    class QtResourceWidget : public QWidget
    {
        Q_OBJECT

        public:
            QtResourceWidget(const QString& iconDir, QWidget* parent = 0);
            virtual ~QtResourceWidget(void);

            // Returns a list of all resources in the resource tree. This includes both groups, subgroups and assets
            // This function can be used to stream the data to a file
            QVector<QtResourceInfo*>& getResources (void);

            // Creates the resourcetree. This includes both groups, subgroups and assets
            // This function can be used to load the data from a file
            void setResources (const QVector<QtResourceInfo*>& resources);

            // Add a material to  the resourcetree
            void addMaterial(const QString& baseNameJson,
                             const QString& fullNameJson,
                             const QString& baseNameThumb,
                             EditorHlmsTypes type);

            // Returns the qualified name of the currently selected item from the resource tree
            const QString& getSelectedFullQualifiedName(void);

        signals:
            // Emitted when a json file is selected to process
            void jSonFileSelectedToProcess(const QString& fullNameJson);

        private slots:
            void handleJsonFileSelectedToProcess(const QString& fullNameJson);

        private:
            QtResourceMain* mResourceMain;
            QString mIconDir;
    };
}

#endif
