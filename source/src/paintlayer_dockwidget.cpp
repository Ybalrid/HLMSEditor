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

// Include
#include <QMessageBox>
#include "OgreRoot.h"
#include "OgreHlmsPbs.h"
#include "OgreHlmsPbsDatablock.h"
#include "OgreHlmsManager.h"
#include "paintlayer_dockwidget.h"
#include "paintlayer_manager.h"
#include "mainwindow.h"
#include "constants.h"

//****************************************************************************/
PaintLayerDockWidget::PaintLayerDockWidget(PaintLayerManager* paintLayerManager,
                                           QString title,
                                           MainWindow* parent,
                                           Qt::WindowFlags flags) :
    QDockWidget (title, parent, flags),
    mParent(parent),
    mPaintLayerManager(paintLayerManager)
{
    mHelperString = "";
    mPaintLayerWidget = new PaintLayerWidget(ICON_PATH, this);
    setWidget(mPaintLayerWidget);
    connect(mPaintLayerWidget, SIGNAL(layerCreatedOrAdded(int,QString)), this, SLOT(handleNewLayer(int,QString)));
    connect(mPaintLayerWidget, SIGNAL(layerDeleted(int,QString)), this, SLOT(handleDeleteLayer(int,QString)));
    connect(mPaintLayerWidget, SIGNAL(layerSelected(int,QString)), this, SLOT(handleLayerSelected(int,QString)));
    connect(mPaintLayerWidget, SIGNAL(layerVisibiltyChanged(int,QString,bool)), this, SLOT(handleLayerVisibilityChanged(int,QString,bool)));
}

//****************************************************************************/
PaintLayerDockWidget::~PaintLayerDockWidget(void)
{
}

//****************************************************************************/
void PaintLayerDockWidget::createPaintLayer(void)
{
    mPaintLayerWidget->createDefaultPaintLayer();
}

//****************************************************************************/
void PaintLayerDockWidget::deleteSelectedPaintLayer(void)
{
    mPaintLayerWidget->deleteSelectedPaintLayer();
}

//****************************************************************************/
bool PaintLayerDockWidget::currentDatablockExists(void)
{
    return (mParent->getCurrentDatablockName() != "");
}

//****************************************************************************/
bool PaintLayerDockWidget::currentDatablockIsPbs(void)
{
    return (mParent->getCurrentDatablockType() == HLMS_PBS);
}

//****************************************************************************/
void PaintLayerDockWidget::newHlmsCreated (void)
{
    // Destroy all PaintLayer objects
    mPaintLayerManager->removeAndDeleteAllPaintLayers();

    // Delete all layers in the PaintL layer widget
    mPaintLayerWidget->deleteAllLayers();
}

//****************************************************************************/
void PaintLayerDockWidget::handleNewLayer (int layerId, const QString& layerName)
{
    // A new layer has been created; create a paint layer by means of the PaintLayerManager
    PaintLayer* paintLayer = mPaintLayerManager->createPaintLayer(layerId);
    if (paintLayer)
        paintLayer->setBrush(BRUSH_PATH + DEFAULT_BRUSH); // Set a default
}

//****************************************************************************/
void PaintLayerDockWidget::handleDeleteLayer (int layerId, const QString& layerName)
{
    // A new layer has been created; delete the paint layer by means of the PaintLayerManager
    mPaintLayerManager->removeAndDeletePaintLayer(layerId);
}

//****************************************************************************/
void PaintLayerDockWidget::setTextureType (int layerId, const QString& textureType)
{
    // Convert texture type to a real Pbs texture type
    Ogre::PbsTextureTypes type;
    if (textureType == PBSM_DIFFUSE_QSTRING)
        type = Ogre::PBSM_DIFFUSE;
    else if (textureType == PBSM_NORMAL_QSTRING)
        type = Ogre::PBSM_NORMAL;
    else if (textureType == PBSM_SPECULAR_QSTRING)
        type = Ogre::PBSM_SPECULAR;
    else if (textureType == PBSM_ROUGHNESS_QSTRING)
        type = Ogre::PBSM_ROUGHNESS;
    else if (textureType == PBSM_DETAIL_WEIGHT_QSTRING)
        type = Ogre::PBSM_DETAIL_WEIGHT;
    else if (textureType == PBSM_DETAIL0_QSTRING)
        type = Ogre::PBSM_DETAIL0;
    else if (textureType == PBSM_DETAIL1_QSTRING)
        type = Ogre::PBSM_DETAIL1;
    else if (textureType == PBSM_DETAIL2_QSTRING)
        type = Ogre::PBSM_DETAIL2;
    else if (textureType == PBSM_DETAIL3_QSTRING)
        type = Ogre::PBSM_DETAIL3;
    else if (textureType == PBSM_DETAIL0_NM_QSTRING)
        type = Ogre::PBSM_DETAIL0_NM;
    else if (textureType == PBSM_DETAIL1_NM_QSTRING)
        type = Ogre::PBSM_DETAIL1_NM;
    else if (textureType == PBSM_DETAIL2_NM_QSTRING)
        type = Ogre::PBSM_DETAIL2_NM;
    else if (textureType == PBSM_DETAIL3_NM_QSTRING)
        type = Ogre::PBSM_DETAIL3_NM;
    else if (textureType == PBSM_REFLECTION_QSTRING)
        type = Ogre::PBSM_REFLECTION;
    else
        return; // Unknown texture type

    // Retrieve the texture filename from the parent
    Ogre::String fileName = mParent->getTextureFileNameOfPbs(mParent->getCurrentDatablockName(), type);
    if (fileName == "")
        return; // Apparently there is no texture with that name

    // Set the datablockName, texturetype and texturefile name in the Paintlayer
    mPaintLayerManager->setTextureLayerInPaintLayer(mParent->getCurrentDatablockName(), type, fileName, layerId);
}

//****************************************************************************/
const QString& PaintLayerDockWidget::getTextureType(int layerId)
{
    mHelperString = "";
    PaintLayer* paintLayer = mPaintLayerManager->getPaintLayer(layerId);
    if (paintLayer)
    {
        TextureLayer* textureLayer = paintLayer->getTextureLayer();
        if (textureLayer)
        {
            Ogre::PbsTextureTypes textureType = textureLayer->mTextureType;
            if (textureType == Ogre::PBSM_DIFFUSE)
                mHelperString = PBSM_DIFFUSE_QSTRING;

            if (textureType == Ogre::PBSM_NORMAL)
                mHelperString = PBSM_NORMAL_QSTRING;

            if (textureType == Ogre::PBSM_SPECULAR)
                mHelperString = PBSM_SPECULAR_QSTRING;

            if (textureType == Ogre::PBSM_ROUGHNESS)
                mHelperString = PBSM_ROUGHNESS_QSTRING;

            if (textureType == Ogre::PBSM_DETAIL_WEIGHT)
                mHelperString = PBSM_DETAIL_WEIGHT_QSTRING;

            if (textureType == Ogre::PBSM_DETAIL0)
                mHelperString = PBSM_DETAIL0_QSTRING;

            if (textureType == Ogre::PBSM_DETAIL1)
                mHelperString = PBSM_DETAIL1_QSTRING;

            if (textureType == Ogre::PBSM_DETAIL2)
                mHelperString = PBSM_DETAIL2_QSTRING;

            if (textureType == Ogre::PBSM_DETAIL3)
                mHelperString = PBSM_DETAIL3_QSTRING;

            if (textureType == Ogre::PBSM_DETAIL0_NM)
                mHelperString = PBSM_DETAIL0_NM_QSTRING;

            if (textureType == Ogre::PBSM_DETAIL1_NM)
                mHelperString = PBSM_DETAIL1_NM_QSTRING;

            if (textureType == Ogre::PBSM_DETAIL2_NM)
                mHelperString = PBSM_DETAIL2_NM_QSTRING;

            if (textureType == Ogre::PBSM_DETAIL3_NM)
                mHelperString = PBSM_DETAIL3_NM_QSTRING;

            if (textureType == Ogre::PBSM_REFLECTION)
                mHelperString = PBSM_REFLECTION_QSTRING;
        }
    }

    return mHelperString;
}

//****************************************************************************/
QStringList PaintLayerDockWidget::getAvailableTextureTypes(void)
{
    // Run through the datablock and check which texture types are used
    mAvailableTextureTypes.clear();

    if (mParent->getCurrentDatablockName() == "")
        return mAvailableTextureTypes;

    // Find the datablock and validate the texture types
    Ogre::HlmsManager* hlmsManager = Ogre::Root::getSingletonPtr()->getHlmsManager();
    Ogre::HlmsPbs* hlmsPbs = static_cast<Ogre::HlmsPbs*>( hlmsManager->getHlms(Ogre::HLMS_PBS));
    Ogre::HlmsDatablock* datablock = 0;
    try
    {
        datablock = hlmsPbs->getDatablock(mParent->getCurrentDatablockName());
        if (datablock)
        {
            // Get the textures
            Ogre::HlmsPbsDatablock* pbsDatablock = static_cast<Ogre::HlmsPbsDatablock*>(datablock);
            if (!pbsDatablock->getTexture(Ogre::PBSM_DIFFUSE).isNull())
                mAvailableTextureTypes << PBSM_DIFFUSE_QSTRING;

            if (!pbsDatablock->getTexture(Ogre::PBSM_NORMAL).isNull())
                mAvailableTextureTypes << PBSM_NORMAL_QSTRING;

            if (!pbsDatablock->getTexture(Ogre::PBSM_SPECULAR).isNull())
                mAvailableTextureTypes << PBSM_SPECULAR_QSTRING;

            if (!pbsDatablock->getTexture(Ogre::PBSM_ROUGHNESS).isNull())
                mAvailableTextureTypes << PBSM_ROUGHNESS_QSTRING;

            if (!pbsDatablock->getTexture(Ogre::PBSM_DETAIL_WEIGHT).isNull())
                mAvailableTextureTypes << PBSM_DETAIL_WEIGHT_QSTRING;

            if (!pbsDatablock->getTexture(Ogre::PBSM_DETAIL0).isNull())
                mAvailableTextureTypes << PBSM_DETAIL0_QSTRING;

            if (!pbsDatablock->getTexture(Ogre::PBSM_DETAIL1).isNull())
                mAvailableTextureTypes << PBSM_DETAIL1_QSTRING;

            if (!pbsDatablock->getTexture(Ogre::PBSM_DETAIL2).isNull())
                mAvailableTextureTypes << PBSM_DETAIL2_QSTRING;

            if (!pbsDatablock->getTexture(Ogre::PBSM_DETAIL3).isNull())
                mAvailableTextureTypes << PBSM_DETAIL3_QSTRING;

            if (!pbsDatablock->getTexture(Ogre::PBSM_DETAIL0_NM).isNull())
                mAvailableTextureTypes << PBSM_DETAIL0_NM_QSTRING;

            if (!pbsDatablock->getTexture(Ogre::PBSM_DETAIL1_NM).isNull())
                mAvailableTextureTypes << PBSM_DETAIL1_NM_QSTRING;

            if (!pbsDatablock->getTexture(Ogre::PBSM_DETAIL2_NM).isNull())
                mAvailableTextureTypes << PBSM_DETAIL2_NM_QSTRING;

            if (!pbsDatablock->getTexture(Ogre::PBSM_DETAIL3_NM).isNull())
                mAvailableTextureTypes << PBSM_DETAIL3_NM_QSTRING;

            if (!pbsDatablock->getTexture(Ogre::PBSM_REFLECTION).isNull())
                mAvailableTextureTypes << PBSM_REFLECTION_QSTRING;
        }
    }
    catch (Ogre::Exception e) {}

    return mAvailableTextureTypes;
}

//****************************************************************************/
void PaintLayerDockWidget::setPaintEffect(int layerId, const QString& paintEffect)
{
    PaintLayer* paintLayer = mPaintLayerManager->getPaintLayer(layerId);
    if (paintLayer)
    {
        if (paintEffect == PAINT_EFFECT_COLOR_QSTRING)
            paintLayer->setPaintEffect (PaintLayer::PAINT_EFFECT_COLOR);
        else if (paintEffect == PAINT_EFFECT_ALPHA_QSTRING)
            paintLayer->setPaintEffect (PaintLayer::PAINT_EFFECT_ALPHA);
        else if (paintEffect == PAINT_EFFECT_TEXTURE_QSTRING)
            paintLayer->setPaintEffect (PaintLayer::PAINT_EFFECT_TEXTURE);
    }
}

//****************************************************************************/
const QString& PaintLayerDockWidget::getPaintEffect(int layerId)
{
    mHelperString = "";
    PaintLayer* paintLayer = mPaintLayerManager->getPaintLayer(layerId);
    if (paintLayer)
    {
        switch (paintLayer->getPaintEffect())
        {
            case PaintLayer::PAINT_EFFECT_COLOR:
                mHelperString = PAINT_EFFECT_COLOR_QSTRING;
            break;
            case PaintLayer::PAINT_EFFECT_ALPHA:
                mHelperString = PAINT_EFFECT_ALPHA_QSTRING;
            break;
            case PaintLayer::PAINT_EFFECT_TEXTURE:
                mHelperString = PAINT_EFFECT_TEXTURE_QSTRING;
            break;
        }
    }
    return mHelperString;
}

//****************************************************************************/
void PaintLayerDockWidget::setPaintOverflow(int layerId, const QString& paintOverflow)
{
    PaintLayer* paintLayer = mPaintLayerManager->getPaintLayer(layerId);
    if (paintLayer)
    {
        if (paintOverflow == PAINT_OVERFLOW_IGNORE_QSTRING)
            paintLayer->setPaintOverflow (PaintLayer::PAINT_OVERFLOW_IGNORE);
        else if (paintOverflow == PAINT_OVERFLOW_CONTINUE_QSTRING)
            paintLayer->setPaintOverflow (PaintLayer::PAINT_OVERFLOW_CONTINUE);
    }
}

//****************************************************************************/
const QString& PaintLayerDockWidget::getPaintOverflow(int layerId)
{
    mHelperString = "";
    PaintLayer* paintLayer = mPaintLayerManager->getPaintLayer(layerId);
    if (paintLayer)
    {
        if (paintLayer->getPaintOverflow() == PaintLayer::PAINT_OVERFLOW_IGNORE)
            mHelperString = PAINT_OVERFLOW_IGNORE_QSTRING;
        else if (paintLayer->getPaintOverflow() == PaintLayer::PAINT_OVERFLOW_CONTINUE)
            mHelperString = PAINT_OVERFLOW_CONTINUE_QSTRING;
    }

    return mHelperString;
}

//****************************************************************************/
void PaintLayerDockWidget::setPaintColour(int layerId, QColor colour)
{
    PaintLayer* paintLayer = mPaintLayerManager->getPaintLayer(layerId);
    if (paintLayer)
    {
        Ogre::ColourValue col;
        col.r = colour.red() / 255.0f;
        col.g = colour.green() / 255.0f;
        col.b = colour.blue() / 255.0f;
        col.a = colour.alpha() / 255.0f;
        paintLayer->setPaintColour(col);
        paintLayer->setJitterPaintEnabled(false); // To stop the jitter
    }
}

//****************************************************************************/
QColor PaintLayerDockWidget::getPaintColour(int layerId)
{
    mHelplerColour = Qt::white;
    PaintLayer* paintLayer = mPaintLayerManager->getPaintLayer(layerId);
    if (paintLayer)
    {
        Ogre::ColourValue col = paintLayer->getPaintColour();
        mHelplerColour.setRed(255.0f * col.r);
        mHelplerColour.setGreen(255.0f * col.g);
        mHelplerColour.setBlue(255.0f * col.b);
        mHelplerColour.setAlpha(255.0f * col.a);
    }

    return mHelplerColour;
}

//****************************************************************************/
bool PaintLayerDockWidget::hasJitterPaintEnabled(int layerId)
{
    bool jitter = false;
    PaintLayer* paintLayer = mPaintLayerManager->getPaintLayer(layerId);
    if (paintLayer)
        jitter = paintLayer->hasJitterPaintEnabled();

    return jitter;
}

//****************************************************************************/
void PaintLayerDockWidget::setJitterPaintColourMin(int layerId, QColor min)
{
    PaintLayer* paintLayer = mPaintLayerManager->getPaintLayer(layerId);
    if (paintLayer)
    {
        Ogre::ColourValue col;
        col.r = min.red() / 255.0f;
        col.g = min.green() / 255.0f;
        col.b = min.blue() / 255.0f;
        col.a = min.alpha() / 255.0f;
        paintLayer->setJitterPaintColourMin(col);
    }
}

//****************************************************************************/
QColor PaintLayerDockWidget::getJitterPaintColourMin(int layerId)
{
    mHelplerColour = Qt::black;
    PaintLayer* paintLayer = mPaintLayerManager->getPaintLayer(layerId);
    if (paintLayer)
    {
        Ogre::ColourValue col = paintLayer->getJitterPaintColourMin();
        mHelplerColour.setRed(255.0f * col.r);
        mHelplerColour.setGreen(255.0f * col.g);
        mHelplerColour.setBlue(255.0f * col.b);
        mHelplerColour.setAlpha(255.0f * col.a);
    }

    return mHelplerColour;
}

//****************************************************************************/
void PaintLayerDockWidget::setJitterPaintColourMax(int layerId, QColor max)
{
    PaintLayer* paintLayer = mPaintLayerManager->getPaintLayer(layerId);
    if (paintLayer)
    {
        Ogre::ColourValue col;
        col.r = max.red() / 255.0f;
        col.g = max.green() / 255.0f;
        col.b = max.blue() / 255.0f;
        col.a = max.alpha() / 255.0f;
        paintLayer->setJitterPaintColourMax(col);
    }
}

//****************************************************************************/
QColor PaintLayerDockWidget::getJitterPaintColourMax(int layerId)
{
    mHelplerColour = Qt::black;
    PaintLayer* paintLayer = mPaintLayerManager->getPaintLayer(layerId);
    if (paintLayer)
    {
        Ogre::ColourValue col = paintLayer->getJitterPaintColourMax();
        mHelplerColour.setRed(255.0f * col.r);
        mHelplerColour.setGreen(255.0f * col.g);
        mHelplerColour.setBlue(255.0f * col.b);
        mHelplerColour.setAlpha(255.0f * col.a);
    }

    return mHelplerColour;
}

//****************************************************************************/
void PaintLayerDockWidget::setJitterPaintColourInterval(int layerId, float interval)
{
    PaintLayer* paintLayer = mPaintLayerManager->getPaintLayer(layerId);
    if (paintLayer)
        paintLayer->setJitterPaintColourInterval(interval);
}

//****************************************************************************/
float PaintLayerDockWidget::getJitterPaintColourInterval(int layerId)
{
    float interval = 0.0f;
    PaintLayer* paintLayer = mPaintLayerManager->getPaintLayer(layerId);
    if (paintLayer)
        interval = paintLayer->getJitterPaintColourInterval();

    return interval;
}

//****************************************************************************/
void PaintLayerDockWidget::setBrushForce(int layerId, float force)
{
    PaintLayer* paintLayer = mPaintLayerManager->getPaintLayer(layerId);
    if (paintLayer)
    {
        paintLayer->setForce(force);
        paintLayer->setJitterForceEnabled(false); // To stop the jitter
    }
}

//****************************************************************************/
float PaintLayerDockWidget::getBrushForce(int layerId)
{
    float force = 0.1f;
    PaintLayer* paintLayer = mPaintLayerManager->getPaintLayer(layerId);
    if (paintLayer)
        force = paintLayer->getForce();

    return force;
}

//****************************************************************************/
bool PaintLayerDockWidget::hasJitterForceEnabled(int layerId)
{
    bool jitter = false;
    PaintLayer* paintLayer = mPaintLayerManager->getPaintLayer(layerId);
    if (paintLayer)
        jitter = paintLayer->hasJitterForceEnabled();

    return jitter;
}

//****************************************************************************/
void PaintLayerDockWidget::setJitterForceMin(int layerId, float min)
{
    PaintLayer* paintLayer = mPaintLayerManager->getPaintLayer(layerId);
    if (paintLayer)
        paintLayer->setJitterForceMin(min);
}

//****************************************************************************/
void PaintLayerDockWidget::setJitterForceMax(int layerId, float max)
{
    PaintLayer* paintLayer = mPaintLayerManager->getPaintLayer(layerId);
    if (paintLayer)
        paintLayer->setJitterForceMax(max);
}

//****************************************************************************/
float PaintLayerDockWidget::getJitterForceMin(int layerId)
{
    float force = 0.1f;
    PaintLayer* paintLayer = mPaintLayerManager->getPaintLayer(layerId);
    if (paintLayer)
        force = paintLayer->getJitterForceMin();

    return force;
}

//****************************************************************************/
float PaintLayerDockWidget::getJitterForceMax(int layerId)
{
    float force = 1.0f;
    PaintLayer* paintLayer = mPaintLayerManager->getPaintLayer(layerId);
    if (paintLayer)
        force = paintLayer->getJitterForceMax();

    return force;
}

//****************************************************************************/
void PaintLayerDockWidget::setJitterForceInterval(int layerId, float interval)
{
    PaintLayer* paintLayer = mPaintLayerManager->getPaintLayer(layerId);
    if (paintLayer)
        paintLayer->setJitterForceInterval(interval);
}

//****************************************************************************/
float PaintLayerDockWidget::getJitterForceInterval(int layerId)
{
    float interval = 0.0f;
    PaintLayer* paintLayer = mPaintLayerManager->getPaintLayer(layerId);
    if (paintLayer)
        interval = paintLayer->getJitterForceInterval();

    return interval;
}

//****************************************************************************/
void PaintLayerDockWidget::setBrushScale(int layerId, float scale)
{
    PaintLayer* paintLayer = mPaintLayerManager->getPaintLayer(layerId);
    if (paintLayer)
    {
        paintLayer->setScale(scale);
        paintLayer->setJitterScaleEnabled(false); // To stop the jitter
    }
}

//****************************************************************************/
float PaintLayerDockWidget::getBrushScale(int layerId)
{
    float scale = 0.1f;
    PaintLayer* paintLayer = mPaintLayerManager->getPaintLayer(layerId);
    if (paintLayer)
        scale = paintLayer->getScale();

    return scale;
}

//****************************************************************************/
bool PaintLayerDockWidget::hasJitterScaleEnabled(int layerId)
{
    bool jitter = false;
    PaintLayer* paintLayer = mPaintLayerManager->getPaintLayer(layerId);
    if (paintLayer)
        jitter = paintLayer->hasJitterScaleEnabled();

    return jitter;
}

//****************************************************************************/
void PaintLayerDockWidget::setJitterScaleMin(int layerId, float min)
{
    PaintLayer* paintLayer = mPaintLayerManager->getPaintLayer(layerId);
    if (paintLayer)
        paintLayer->setJitterScaleMin(min);
}

//****************************************************************************/
void PaintLayerDockWidget::setJitterScaleMax(int layerId, float max)
{
    PaintLayer* paintLayer = mPaintLayerManager->getPaintLayer(layerId);
    if (paintLayer)
        paintLayer->setJitterScaleMax(max);
}

//****************************************************************************/
float PaintLayerDockWidget::getJitterScaleMin(int layerId)
{
    float scale = 0.1f;
    PaintLayer* paintLayer = mPaintLayerManager->getPaintLayer(layerId);
    if (paintLayer)
        scale = paintLayer->getJitterScaleMin();

    return scale;
}

//****************************************************************************/
float PaintLayerDockWidget::getJitterScaleMax(int layerId)
{
    float scale = 0.1f;
    PaintLayer* paintLayer = mPaintLayerManager->getPaintLayer(layerId);
    if (paintLayer)
        scale = paintLayer->getJitterScaleMax();

    return scale;
}

//****************************************************************************/
void PaintLayerDockWidget::setJitterScaleInterval(int layerId, float interval)
{
    PaintLayer* paintLayer = mPaintLayerManager->getPaintLayer(layerId);
    if (paintLayer)
        paintLayer->setJitterScaleInterval(interval);
}

//****************************************************************************/
float PaintLayerDockWidget::getJitterScaleInterval(int layerId)
{
    float interval = 0.1f;
    PaintLayer* paintLayer = mPaintLayerManager->getPaintLayer(layerId);
    if (paintLayer)
        interval = paintLayer->getJitterScaleInterval();

    return interval;
}

//****************************************************************************/
void PaintLayerDockWidget::setRotationAngle(int layerId, float angle)
{
    PaintLayer* paintLayer = mPaintLayerManager->getPaintLayer(layerId);
    if (paintLayer)
    {
        paintLayer->setRotationAngle(angle);
        paintLayer->setJitterRotationEnabled(false); // To stop the jitter
    }
}

//****************************************************************************/
float PaintLayerDockWidget::getRotationAngle(int layerId)
{
    float angle = 0.0f;
    PaintLayer* paintLayer = mPaintLayerManager->getPaintLayer(layerId);
    if (paintLayer)
        angle = paintLayer->getRotationAngle();

    return angle;
}

//****************************************************************************/
bool PaintLayerDockWidget::hasJitterRotationAngleEnabled(int layerId)
{
    bool jitter = false;
    PaintLayer* paintLayer = mPaintLayerManager->getPaintLayer(layerId);
    if (paintLayer)
        jitter = paintLayer->hasJitterRotationEnabled();

    return jitter;
}

//****************************************************************************/
void PaintLayerDockWidget::setJitterRotationAngleMin(int layerId, float min)
{
    PaintLayer* paintLayer = mPaintLayerManager->getPaintLayer(layerId);
    if (paintLayer)
        paintLayer->setJitterRotationAngleMin(min);
}

//****************************************************************************/
float PaintLayerDockWidget::getJitterRotationAngleMin(int layerId)
{
    float min = 0.0f;
    PaintLayer* paintLayer = mPaintLayerManager->getPaintLayer(layerId);
    if (paintLayer)
        min = paintLayer->getJitterRotationAngleMin();

    return min;
}

//****************************************************************************/
void PaintLayerDockWidget::setJitterRotationAngleMax(int layerId, float max)
{
    PaintLayer* paintLayer = mPaintLayerManager->getPaintLayer(layerId);
    if (paintLayer)
        paintLayer->setJitterRotationAngleMax(max);
}

//****************************************************************************/
float PaintLayerDockWidget::getJitterRotationAngleMax(int layerId)
{
    float max = 360.0f;
    PaintLayer* paintLayer = mPaintLayerManager->getPaintLayer(layerId);
    if (paintLayer)
        max = paintLayer->getJitterRotationAngleMax();

    return max;
}

//****************************************************************************/
void PaintLayerDockWidget::setJitterRotationAngleInterval(int layerId, float interval)
{
    PaintLayer* paintLayer = mPaintLayerManager->getPaintLayer(layerId);
    if (paintLayer)
        paintLayer->setJitterRotationAngleInterval(interval);
}

//****************************************************************************/
float PaintLayerDockWidget::getJitterRotationAngleInterval(int layerId)
{
    float interval = 0.0f;
    PaintLayer* paintLayer = mPaintLayerManager->getPaintLayer(layerId);
    if (paintLayer)
        interval = paintLayer->getJitterRotationAngleInterval();

    return interval;
}

//****************************************************************************/
void PaintLayerDockWidget::setTranslation(int layerId, const QVector2D& translation)
{
    PaintLayer* paintLayer = mPaintLayerManager->getPaintLayer(layerId);
    if (paintLayer)
    {
        paintLayer->setTranslationFactor(translation.x(), translation.y());
        paintLayer->setJitterTranslationEnabled(false); // To stop the jitter
    }
}

//****************************************************************************/
const QVector2D& PaintLayerDockWidget::getTranslation(int layerId)
{
    mHelperVector2D.setX(0.0f);
    mHelperVector2D.setY(0.0f);
    PaintLayer* paintLayer = mPaintLayerManager->getPaintLayer(layerId);
    if (paintLayer)
    {
        mHelperVector2D.setX(paintLayer->getTranslationFactorX());
        mHelperVector2D.setY(paintLayer->getTranslationFactorY());
    }
    return mHelperVector2D;
}

//****************************************************************************/
bool PaintLayerDockWidget::hasJitterTranslationEnabled(int layerId)
{
    bool enabled;
    PaintLayer* paintLayer = mPaintLayerManager->getPaintLayer(layerId);
    if (paintLayer)
        enabled = paintLayer->hasJitterTranslationEnabled();

    return enabled;
}

//****************************************************************************/
void PaintLayerDockWidget::setJitterTranslationX(int layerId, const QVector2D& xMinMax)
{
    PaintLayer* paintLayer = mPaintLayerManager->getPaintLayer(layerId);
    if (paintLayer)
        paintLayer->setJitterTranslationFactorX(xMinMax.x(), xMinMax.y());
}

//****************************************************************************/
const QVector2D&  PaintLayerDockWidget::getJitterTranslationX(int layerId)
{
    mHelperVector2D.setX(0.0f);
    mHelperVector2D.setY(0.0f);
    PaintLayer* paintLayer = mPaintLayerManager->getPaintLayer(layerId);
    if (paintLayer)
    {
        mHelperVector2D.setX(paintLayer->getJitterTranslationFactorX().x); // xMin
        mHelperVector2D.setY(paintLayer->getJitterTranslationFactorX().y); // xMax
    }
    return mHelperVector2D;
}

//****************************************************************************/
void PaintLayerDockWidget::setJitterTranslationY(int layerId, const QVector2D& yMinMax)
{
    PaintLayer* paintLayer = mPaintLayerManager->getPaintLayer(layerId);
    if (paintLayer)
        paintLayer->setJitterTranslationFactorY(yMinMax.x(), yMinMax.y());
}

//****************************************************************************/
const QVector2D& PaintLayerDockWidget::getJitterTranslationY(int layerId)
{
    mHelperVector2D.setX(0.0f);
    mHelperVector2D.setY(0.0f);
    PaintLayer* paintLayer = mPaintLayerManager->getPaintLayer(layerId);
    if (paintLayer)
    {
        mHelperVector2D.setX(paintLayer->getJitterTranslationFactorY().x); // yMin
        mHelperVector2D.setY(paintLayer->getJitterTranslationFactorY().y); // yMax
    }
    return mHelperVector2D;
}

//****************************************************************************/
void PaintLayerDockWidget::setJitterTranslationInterval(int layerId, float interval)
{
    PaintLayer* paintLayer = mPaintLayerManager->getPaintLayer(layerId);
    if (paintLayer)
        paintLayer->setJitterTranslationInterval(interval);
}

//****************************************************************************/
float PaintLayerDockWidget::getJitterTranslationInterval(int layerId)
{
    float interval = 0.0f;
    PaintLayer* paintLayer = mPaintLayerManager->getPaintLayer(layerId);
    if (paintLayer)
        interval = paintLayer->getJitterTranslationInterval();

    return interval;
}

//****************************************************************************/
void PaintLayerDockWidget::setMirrorHorizontal(int layerId, bool mirrored)
{
    PaintLayer* paintLayer = mPaintLayerManager->getPaintLayer(layerId);
    if (paintLayer)
    {
        paintLayer->setMirrorHorizontal(mirrored);
        paintLayer->setJitterMirrorHorizontalEnabled(false); // To stop the jitter
    }
}

//****************************************************************************/
bool PaintLayerDockWidget::getMirrorHorizontal(int layerId)
{
    bool enabled = false;
    PaintLayer* paintLayer = mPaintLayerManager->getPaintLayer(layerId);
    if (paintLayer)
        enabled = paintLayer->getMirrorHorizontal();

    return enabled;
}

//****************************************************************************/
void PaintLayerDockWidget::setJitterMirrorHorizontal(int layerId, bool enabled)
{
    PaintLayer* paintLayer = mPaintLayerManager->getPaintLayer(layerId);
    if (paintLayer)
        paintLayer->setJitterMirrorHorizontalEnabled(enabled);
}

//****************************************************************************/
bool PaintLayerDockWidget::hasJitterMirrorHorizontal(int layerId)
{
    bool enabled = false;
    PaintLayer* paintLayer = mPaintLayerManager->getPaintLayer(layerId);
    if (paintLayer)
        enabled = paintLayer->hasJitterMirrorHorizontalEnabled();

    return enabled;
}

//****************************************************************************/
void PaintLayerDockWidget::setJitterMirrorHorizontalInterval(int layerId, float interval)
{
    PaintLayer* paintLayer = mPaintLayerManager->getPaintLayer(layerId);
    if (paintLayer)
        paintLayer->setJitterMirrorHorizontalInterval(interval);
}

//****************************************************************************/
float PaintLayerDockWidget::getJitterMirrorHorizontalInterval(int layerId)
{
    float interval = 0.0f;
    PaintLayer* paintLayer = mPaintLayerManager->getPaintLayer(layerId);
    if (paintLayer)
        interval = paintLayer->getJitterMirrorHorizontalInterval();

    return interval;
}

//****************************************************************************/
void PaintLayerDockWidget::setMirrorVertical(int layerId, bool mirrored)
{
    PaintLayer* paintLayer = mPaintLayerManager->getPaintLayer(layerId);
    if (paintLayer)
    {
        paintLayer->setMirrorVertical(mirrored);
        paintLayer->setJitterMirrorVerticalEnabled(false); // To stop the jitter
    }
}

//****************************************************************************/
bool PaintLayerDockWidget::getMirrorVertical(int layerId)
{
    bool enabled = false;
    PaintLayer* paintLayer = mPaintLayerManager->getPaintLayer(layerId);
    if (paintLayer)
        enabled = paintLayer->getMirrorVertical();

    return enabled;
}

//****************************************************************************/
void PaintLayerDockWidget::setJitterMirrorVertical(int layerId, bool enabled)
{
    PaintLayer* paintLayer = mPaintLayerManager->getPaintLayer(layerId);
    if (paintLayer)
        paintLayer->setJitterMirrorVerticalEnabled(enabled);
}

//****************************************************************************/
bool PaintLayerDockWidget::hasJitterMirrorVertical(int layerId)
{
    bool enabled = false;
    PaintLayer* paintLayer = mPaintLayerManager->getPaintLayer(layerId);
    if (paintLayer)
        enabled = paintLayer->hasJitterMirrorVerticalEnabled();

    return enabled;
}

//****************************************************************************/
void PaintLayerDockWidget::setJitterMirrorVerticalInterval(int layerId, float interval)
{
    PaintLayer* paintLayer = mPaintLayerManager->getPaintLayer(layerId);
    if (paintLayer)
        paintLayer->setJitterMirrorVerticalInterval(interval);
}

//****************************************************************************/
float PaintLayerDockWidget::getJitterMirrorVerticalInterval(int layerId)
{
    float interval = 0.0f;
    PaintLayer* paintLayer = mPaintLayerManager->getPaintLayer(layerId);
    if (paintLayer)
        interval = paintLayer->getJitterMirrorVerticalInterval();

    return interval;
}


//****************************************************************************/
void PaintLayerDockWidget::handleLayerSelected (int layerId, const QString& layerName)
{
    // Get the selected layers
    QVector<int> v = mPaintLayerWidget->getSelectedLayerIds();
    mPaintLayerManager->enableAllPaintLayers(false); // First disable everything
    mPaintLayerManager->enablePaintLayers (v.toStdVector(), true); // Enable every selected layer
}

//****************************************************************************/
void PaintLayerDockWidget::handleLayerVisibilityChanged(int layerId, const QString& name, bool visible)
{
    PaintLayer* paintLayer = mPaintLayerManager->getPaintLayer(layerId);
    TextureLayer* textureLayer = paintLayer->getTextureLayer();
    if (!textureLayer)
        return;

    if (!textureLayer->mTextureTypeDefined)
        return;

    Ogre::PbsTextureTypes type = textureLayer->mTextureType;
    QVector<QtLayer*> v = mPaintLayerWidget->getLayers();
    foreach (QtLayer* layer, v)
    {
        paintLayer = mPaintLayerManager->getPaintLayer(layer->layerId);
        textureLayer = paintLayer->getTextureLayer();
        if (textureLayer &&
                textureLayer->mTextureTypeDefined &&
                textureLayer->mTextureType == type)
        {
            // Set the icon
            mPaintLayerWidget->updateVisibilityIconForLayerId(layer->layerId, visible, false);

            // Disable the paintlayer from painting if the icon is set to invisible; enable it again
            // if the icon is visible.
            paintLayer->setVisible(visible);
        }
    }

    // Set to the correct texture. The first texture generation is the original texture and the
    // last texture generation is the texture on which is painted.
    if (visible)
        textureLayer->setLastTextureGeneration();
    else
        textureLayer->setFirstTextureGeneration();
}

//****************************************************************************/
void PaintLayerDockWidget::setBrushInPaintLayer(const QString& name, const QString& baseName)
{
    int layerId = mPaintLayerWidget->getCurrentLayerId();
    if (layerId < 0)
        return;

    PaintLayer* paintLayer = mPaintLayerManager->getPaintLayer(layerId);
    if (paintLayer)
    {
        paintLayer->setBrush(baseName.toStdString());
        mPaintLayerWidget->setBrushIconInCurrentLayers(name);
    }
}
