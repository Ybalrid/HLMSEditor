/****************************************************************************
**
** Copyright (C) 2016 - 2017
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

// Include
#include <QTextStream>
#include "constants.h"
#include "mainwindow.h"
#include "properties_dockwidget.h"

//****************************************************************************/
PropertiesDockWidget::PropertiesDockWidget(QString title, MainWindow* parent, Qt::WindowFlags flags) : 
	QDockWidget (title, parent, flags), 
	mParent(parent)
{
    mTextViewer = new QPlainTextEdit();
    mTextViewer->hide();
    mTextViewer->setReadOnly(true);
    mTextViewer->setWindowFlags(Qt::WindowStaysOnTopHint);
    mTextViewer->setTextInteractionFlags(mTextViewer->textInteractionFlags() & Qt::TextSelectableByKeyboard);
    mTextViewer->setLineWrapMode(QPlainTextEdit::WidgetWidth);
    QFont font("Courier", 9);
    mTextViewer->setFont(font);

    mInnerMain = new QMainWindow();
    mInnerMain->setMinimumSize(100, 100);
    setWidget(mInnerMain);

    // Perform standard functions
    createActions();
    createMenus();
    createToolBars();

    mHlmsPropertiesPbsDatablock = new HlmsPropertiesPbsDatablock(ICON_PBS_DATABLOCK, this, mInnerMain);
    mHlmsPropertiesUnlitDatablock = new HlmsPropertiesUnlitDatablock(ICON_UNLIT_DATABLOCK, this, mInnerMain);
    mHlmsPropertiesBlendblock = new HlmsPropertiesBlendblock(ICON_BLENDBLOCK, this, mInnerMain);
    mHlmsPropertiesMacroblock = new HlmsPropertiesMacroblock(ICON_MACROBLOCK, this, mInnerMain);
    mHlmsPropertiesSamplerblock = new HlmsPropertiesSamplerblock(ICON_SAMPLERBLOCK, this, mInnerMain);

    // Make all property windows invisible
    clear();

    // Make some space
    setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding);
    QRect rec = QApplication::desktop()->screenGeometry();
    setMinimumWidth(0.2 * rec.width());
    //layout()->setSizeConstraint(QLayout::SetMinAndMaxSize);

    mHlmsPropertiesPbsDatablock->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    mHlmsPropertiesPbsDatablock->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    mHlmsPropertiesUnlitDatablock->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    mHlmsPropertiesBlendblock->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    mHlmsPropertiesMacroblock->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    mHlmsPropertiesSamplerblock->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
}

//****************************************************************************/
PropertiesDockWidget::~PropertiesDockWidget(void)
{
}

//****************************************************************************/
void PropertiesDockWidget::createActions(void)
{

}

//****************************************************************************/
void PropertiesDockWidget::createMenus(void)
{

}

//****************************************************************************/
void PropertiesDockWidget::createToolBars(void)
{

}

//****************************************************************************/
void PropertiesDockWidget::setHlmsPropertiesPbsDatablockVisible(bool visible)
{
    setWidgetVisible (mHlmsPropertiesPbsDatablock, visible);
}

//****************************************************************************/
void PropertiesDockWidget::setHlmsPropertiesUnlitDatablockVisible(bool visible)
{
    setWidgetVisible (mHlmsPropertiesUnlitDatablock, visible);
}

//****************************************************************************/
void PropertiesDockWidget::setHlmsPropertiesBlendblockVisible(bool visible)
{
    setWidgetVisible (mHlmsPropertiesBlendblock, visible);
}

//****************************************************************************/
void PropertiesDockWidget::setHlmsPropertiesMacroblockVisible(bool visible)
{
    setWidgetVisible (mHlmsPropertiesMacroblock, visible);
}

//****************************************************************************/
void PropertiesDockWidget::setHlmsPropertiesSamplerblockVisible(bool visible)
{
    setWidgetVisible (mHlmsPropertiesSamplerblock, visible);
}

//****************************************************************************/
void PropertiesDockWidget::clear()
{
    // Make all property windows invisible
    mHlmsPropertiesPbsDatablock->setVisible(false);
    mHlmsPropertiesUnlitDatablock->setVisible(false);
    mHlmsPropertiesBlendblock->setVisible(false);
    mHlmsPropertiesMacroblock->setVisible(false);
    mHlmsPropertiesSamplerblock->setVisible(false);
}

//****************************************************************************/
void PropertiesDockWidget::setTextureTypePropertyVisible(bool visible)
{
    mHlmsPropertiesSamplerblock->setTextureTypePropertyVisible(visible);
}

//****************************************************************************/
void PropertiesDockWidget::setDetailMapWOSPropertiesVisible(bool visible)
{
    mHlmsPropertiesSamplerblock->setDetailMapWOSPropertiesVisible(visible);
}

//****************************************************************************/
void PropertiesDockWidget::setDetailMapAnimationPropertiesVisible(bool visible)
{
    mHlmsPropertiesSamplerblock->setDetailMapAnimationPropertiesVisible(visible);
}

//****************************************************************************/
void PropertiesDockWidget::displayInfo(const QString& fileName, const QString& headerText)
{
    QFile file(fileName);
    if(!file.exists())
        return;

    setCursor(Qt::WaitCursor);
    QString line;
    mTextViewer->clear();
    if (file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        QTextStream stream(&file);
        while (!stream.atEnd())
        {
            line = stream.readLine();
            mTextViewer->appendPlainText(line);
        }
    }
    file.close();
    mTextViewer->moveCursor(QTextCursor::Start);
    mTextViewer->setFocus();
    mTextViewer->setWindowTitle(headerText);
    mTextViewer->show();
    setCursor(Qt::ArrowCursor);
}

//****************************************************************************/
void PropertiesDockWidget::notifyMaterialChanged(QtProperty* property)
{
    mParent->notifyMaterialChanged(property);
}


//****************************************************************************/
void PropertiesDockWidget::setWidgetVisible (QWidget* widget, bool visible)
{
    widget->setVisible(visible);
    if (visible)
    {
        QRect rec = QApplication::desktop()->screenGeometry();
        setMaximumSize(rec.size());
        setMinimumWidth(0.2 * rec.width());
        setWidget(widget);
        widget->showMaximized();
    }
    else
        setMinimumSize(widget->size());
}

//****************************************************************************/
const QString& PropertiesDockWidget::saveHlmsPropertiesSamplerblock (void)
{
    return mHlmsPropertiesSamplerblock->saveProperties(""); // Let mHlmsPropertiesSamplerblock determine its own filename
}

//****************************************************************************/
const QString& PropertiesDockWidget::saveHlmsPropertiesPbsDatablock (void)
{
    return mHlmsPropertiesPbsDatablock->saveProperties(""); // Let mHlmsPropertiesPbsDatablock determine its own filename
}

//****************************************************************************/
void PropertiesDockWidget::loadHlmsPropertiesSamplerblock (const QString& fileName)
{
    mHlmsPropertiesSamplerblock->loadProperties(fileName); // The fileName is fully qualified
}

//****************************************************************************/
void PropertiesDockWidget::loadHlmsPropertiesPbsDatablock (const QString& fileName)
{
    mHlmsPropertiesPbsDatablock->loadProperties(fileName); // The fileName is fully qualified
}

//****************************************************************************/
bool PropertiesDockWidget::isSamplerProperties (const QString& fileName)
{
    return mHlmsPropertiesSamplerblock->isSamplerProperties(fileName); // The fileName is fully qualified
}

//****************************************************************************/
bool PropertiesDockWidget::isPbsProperties (const QString& fileName)
{
    return mHlmsPropertiesPbsDatablock->isPbsProperties(fileName); // The fileName is fully qualified
}
