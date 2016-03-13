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

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QCloseEvent>
#include <QMainWindow>
#include <QMenu>
#include <QAction>
#include <QMessageBox>
#include <QToolBar>
#include "renderwindow_dockwidget.h"
#include "properties_dockwidget.h"
#include "nodeeditor_dockwidget.h"
#include "browser_dialog.h"
#include "ogre3_renderman.h"
#include "constants.h"

QT_BEGIN_NAMESPACE

QT_END_NAMESPACE

/****************************************************************************
 MainWindow is the main container window
 ***************************************************************************/
class MainWindow : public QMainWindow
{
	Q_OBJECT

	public:
		MainWindow(void);
		~MainWindow(void);
        bool eventFilter(QObject* object, QEvent* event);
		void update(void);
		bool mIsClosing;
        Magus::OgreManager* getOgreManager(void) const {return mOgreManager;}
        PropertiesDockWidget* mPropertiesDockWidget; // Make is public for easy access
        void initDatablocks(void); // To be called by the editor window, to indicate that a new Hlms datablock is created
        void getListOfResources(void); // Function to test which resources are loaded
        void getAndSetFirstDatablock(void); // Get the first datablock from all datablocks and set it to the item in the renderwindow
        void destroyAllDatablocks(void); // Destroy all datablocks
        EditorHlmsTypes getCurrentDatablockType(void); // Returns the current hlms type

	private slots:
        void doNewHlmsPbsAction(void);
        void doNewHlmsUnlitAction(void);
        void doOpenDatablockMenuAction(void);
        void doSaveDatablockMenuAction(void);
        void doSaveAsDatablockMenuAction(void);
        void doMaterialBrowserMenuAction(void);
        void doQuitMenuAction(void);
        void doResetWindowLayoutMenuAction(void);

	private:
		void createActions(void);
		void createMenus(void);
		void createToolBars(void);
		void createStatusBar(void);
		void createDockWindows(void);
		void closeEvent(QCloseEvent* event);
        void loadDatablock(const QString jsonFileName);
        void saveDatablock(void);
        QString getBaseFileName(QString& fileName);

        bool mFirst;
        QString mTempString;
        MaterialBrowserDialog* mMaterialBrowser;
        QMenu* mFileMenu;
        QMenu* mWindowMenu;
        QAction* mNewHlmsPbsAction;
        QAction* mNewHlmsUnlitAction;
        QAction* mOpenDatablockMenuAction;
        QAction* mSaveDatablockMenuAction;
        QAction* mSaveAsDatablockMenuAction;
        QAction* mMaterialBrowserMenuAction;
        QAction* mQuitMenuAction;
        QAction* mResetWindowLayoutMenuAction;
        RenderwindowDockWidget* mRenderwindowDockWidget;
        NodeEditorDockWidget* mNodeEditorDockWidget;
        Magus::OgreManager* mOgreManager;
        QString mHlmsName; // Used to determine whether a hlms was already saved

};

#endif

