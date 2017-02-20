/****************************************************************************
**
** Copyright (C) 2016
**
** This file is part of the Magus toolkit
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

#include "paintlayer.h"
#include "paintlayer_manager.h"
#include "OgreHlmsPbs.h"
#include "OgreHlmsManager.h"
#include "OgreLogManager.h"
#include "OgreStringConverter.h"
#include "constants.h"
#include "math.h"

//****************************************************************************/
PaintLayer::PaintLayer(PaintLayerManager* paintLayerManager, int externalLayerId) :
    mPaintLayerManager(paintLayerManager),
    mExternaLayerlId(externalLayerId),
    mEnabled(true),
    mVisible(true),
    mBrushWidth(0),
    mBrushHeight(0),
    mHalfBrushWidth(0),
    mHalfBrushWidthScaled(0),
    mHalfBrushHeight(0),
    mHalfBrushHeightScaled(0),
    mBrushWidthMinusOne(0),
    mBrushHeightMinusOne(0),
    mForce(1.0f),
    mScale(0.1f),
    mPaintEffect(PAINT_EFFECT_COLOR),
    mPaintOverflow(PAINT_OVERFLOW_CONTINUE),
    mTextureLayer(0),
    calculatedTexturePositionX(0),
    calculatedTexturePositionY(0),
    mAlpha(1.0f),
    mRotate(false),
    mRotationAngle(0.0f),
    mSinRotationAngle(0.0f),
    mCosRotationAngle(0.0f),
    mPosX(0),
    mPosY(0),
    mPosXrotated(0),
    mPosYrotated(0),
    mTranslate(false),
    mTranslationFactorX(0.0f),
    mTranslationFactorY(0.0f),
    mTranslationX(0),
    mTranslationY(0),
    mMirrorHorizontal(false),
    mMirrorVertical(false),
    mJitterRotate(false),
    mJitterTranslate(false),
    mJitterScale(false),
    mJitterForce(false),
    mJitterPaintColour(false),
    mJitterMirrorHorizontal(false),
    mJitterMirrorVertical(false),
    mJitterRotationAngleInterval(0.0f),
    mJitterTranslationInterval(0.0f),
    mJitterScaleInterval(0.0f),
    mJitterForceInterval(0.0f),
    mJitterPaintColourInterval(0.0f),
    mJitterMirrorHorizontalInterval(0.0f),
    mJitterMirrorVerticalInterval(0.0f),
    mJitterElapsedTime(0.0f),
    mJitterRotateElapsedTime(0.0f),
    mJitterTranslationElapsedTime(0.0f),
    mJitterScaleElapsedTime(0.0f),
    mJitterForceElapsedTime(0.0f),
    mJitterPaintColourElapsedTime(0.0f),
    mJitterMirrorHorizontalElapsedTime(0.0f),
    mJitterMirrorVerticalElapsedTime(0.0f),
    mJitterRotationAngleMin(0.0f),
    mJitterRotationAngleMax(360.0f),
    mJitterTranslationFactorXmin(-1),
    mJitterTranslationFactorXmax(1),
    mJitterTranslationFactorYmin(-1),
    mJitterTranslationFactorYmax(1),
    mJitterScaleMin(0.0f),
    mJitterScaleMax(1.0f),
    mJitterForceMin(0.1f),
    mJitterForceMax(1.0f)
{
    mPaintColour = Ogre::ColourValue::White;
    mFinalColour = Ogre::ColourValue::White;
    mJitterPaintColourMin = Ogre::ColourValue::Black;
    mJitterPaintColourMax = Ogre::ColourValue::White;
    mBrushFileName = "";
    dummyDatablockId = "";
    mStartTime = clock();
}

//****************************************************************************/
PaintLayer::~PaintLayer(void)
{
}

//****************************************************************************/
void PaintLayer::enable(bool enabled)
{
    mEnabled = enabled;
}

//****************************************************************************/
void PaintLayer::setVisible (bool visible)
{
    mVisible = visible;
}

//****************************************************************************/
bool PaintLayer::isVisible(void)
{
    return mVisible;
}

//****************************************************************************/
void PaintLayer::paint(float u, float v)
{
    // Apply paint effect if there is a texture
    if (!mEnabled || !mVisible || !mTextureLayer || mTextureLayer->mTexture.isNull())
        return;

    /* If there are jitter effects, they are applied first. This means that certain jitter attributes are set
     * to s specific value, so the brush image is applied to the texture according to these jitter attributes.
     */
    determineJitterEffects();

    /* Loop through the pixelbox of the brush and apply the paint effect to the pixelbox of the texture
     * Note, that only mipmap 0 of the texture image is painted, so prevent textures with mipmaps.
     * The texture on the GPU may contain mipmaps; these ARE painted.
     */
    for (size_t y = 0; y < mBrushHeight; y++)
    {
        for (size_t x = 0; x < mBrushWidth; x++)
        {
            // If mirroring, rotation and/or translation are enabled, calculate the new pixel position of the brush
            mPosX = x;
            mPosY = y;
            if (mMirrorHorizontal)
            {
                mPosY = mBrushHeightMinusOne - mPosY;
            }
            if (mMirrorVertical)
            {
                mPosX = mBrushWidthMinusOne - mPosX;
            }
            if (mRotate)
            {
                // Calculate the rotated pixel position
                mPosX -= mHalfBrushWidth;
                mPosY -= mHalfBrushHeight;
                mPosXrotated = mPosX * mCosRotationAngle - mPosY * mSinRotationAngle;
                mPosYrotated = mPosX * mSinRotationAngle + mPosY * mCosRotationAngle;
                mPosX = mPosXrotated + mHalfBrushWidth;
                mPosY = mPosYrotated + mHalfBrushHeight;

                // Determine whether the pixel position still fits within the brush dimensions
                if (mPosX < 0 || mPosX >= mBrushWidth || mPosY < 0 || mPosY >= mBrushHeight)
                    continue;
            }
            if (mTranslate)
            {
                // Calculate the translated pixel position
                mPosX += mTranslationX;
                mPosY += mTranslationY;

                // Determine whether the pixel position still fits within the brush dimensions
                if (mPosX < 0 || mPosX >= mBrushWidth || mPosY < 0 || mPosY >= mBrushHeight)
                    continue;
            }

            // Calculate the position of the brush pixel to a texture position
            calculatedTexturePositionX = calculateTexturePositionX(u, x);
            calculatedTexturePositionY = calculateTexturePositionY(v, y);

            if (mPaintEffect == PAINT_EFFECT_COLOR)
            {
                // Paint with colour
                mAlpha = mForce * mPixelboxBrush.getColourAt(mPosX, mPosY, 0).a;
                mFinalColour = mAlpha * mPaintColour;
                mFinalColour = (1.0f - mAlpha) * mTextureLayer->mPixelboxTextureOnWhichIsPainted.getColourAt(calculatedTexturePositionX,
                                                                                                             calculatedTexturePositionY,
                                                                                                             0) + mFinalColour;
                mTextureLayer->mPixelboxTextureOnWhichIsPainted.setColourAt(mFinalColour,
                                                                            calculatedTexturePositionX,
                                                                            calculatedTexturePositionY,
                                                                            0);
            }
            else if (mPaintEffect == PAINT_EFFECT_ALPHA)
            {
                // Paint with alpha value of the brush (note, that the texture must have alpha!)
                mAlpha = mForce * mPixelboxBrush.getColourAt(mPosX, mPosY, 0).a;
                mFinalColour = mTextureLayer->mPixelboxTextureOnWhichIsPainted.getColourAt(calculatedTexturePositionX,
                                                                                           calculatedTexturePositionY,
                                                                                           0);

                mFinalColour.a -= mAlpha;
                mTextureLayer->mPixelboxTextureOnWhichIsPainted.setColourAt(mFinalColour,
                                                                            calculatedTexturePositionX,
                                                                            calculatedTexturePositionY,
                                                                            0);
            }
            else if (mPaintEffect == PAINT_EFFECT_TEXTURE)
            {
                // Paint with coloured brush
                Ogre::ColourValue brushColour = mPixelboxBrush.getColourAt(mPosX, mPosY, 0);
                mAlpha = mForce * brushColour.a;
                mFinalColour = (1.0 - mAlpha) * mTextureLayer->mPixelboxTextureOnWhichIsPainted.getColourAt(calculatedTexturePositionX,
                                                                                                            calculatedTexturePositionY,
                                                                                                            0) + mAlpha * brushColour;

                mTextureLayer->mPixelboxTextureOnWhichIsPainted.setColourAt(mFinalColour,
                                                                            calculatedTexturePositionX,
                                                                            calculatedTexturePositionY,
                                                                            0);
            }
        }
    }

    /* Copy the changed texture to the GPU; beware to also update the mipmaps.
     * The texture on the GPU (mTexture) may contain more mipmaps (or less) than the
     * texture loaded as an image (mTextureOnWhichIsPainted), although they refer to the
     * same texture file. Mipsmaps are updated by means of scaling the texture image.
     */
    mTextureLayer->blitTexture();

    /*
    size_t w = mTextureLayer->mTextureOnWhichIsPaintedWidth;
    size_t h = mTextureLayer->mTextureOnWhichIsPaintedHeight;
    Ogre::Image textureOnWhichIsPaintedScaled = mTextureLayer->mTextureOnWhichIsPainted; // Define textureOnWhichIsPaintedScaled each time; reusing results in exception
    for (Ogre::uint8 i = 0; i < mTextureLayer->mNumMipMaps; ++i)
    {
        //mTextureLayer->mBuffers.at(i)->lock(Ogre::v1::HardwareBuffer::HBL_DISCARD);
        mTextureLayer->mBuffers.at(i)->blitFromMemory(textureOnWhichIsPaintedScaled.getPixelBox(0,0), Ogre::Box(0, 0, 0, w, h, 1));
        //mTextureLayer->mBuffers.at(i)->unlock();
        w*=0.5f; // Mipmaps always are half of the previous one
        h*=0.5f;
        if (w > 1.0f && h > 1.0f)
            textureOnWhichIsPaintedScaled.resize(w, h);
        else
            break; // Stop when the mipmaps are too small
    }
    textureOnWhichIsPaintedScaled.freeMemory();
    */
}

//****************************************************************************/
void PaintLayer::setTextureLayer (TextureLayer* textureLayer)
{
    mTextureLayer = textureLayer;
}

//****************************************************************************/
TextureLayer* PaintLayer::getTextureLayer (void)
{
    return mTextureLayer;
}

//****************************************************************************/
void PaintLayer::setBrush (const Ogre::String& brushFileName)
{
    try
    {
        mBrushFileName = brushFileName;
        mBrush.load(brushFileName, Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);
        mPixelboxBrush = mBrush.getPixelBox(0, 0);
        mBrushWidth = mBrush.getWidth();
        mHalfBrushWidth = 0.5f * mBrushWidth;
        mHalfBrushWidthScaled = mScale * mHalfBrushWidth;
        mBrushWidthMinusOne = mBrushWidth - 1;
        mBrushHeight = mBrush.getHeight();
        mHalfBrushHeight = 0.5f * mBrushHeight;
        mHalfBrushHeightScaled = mScale * mHalfBrushHeight;
        mBrushHeightMinusOne = mBrushHeight - 1;

        mTranslationX = mTranslationFactorX * mBrushWidth;
        mTranslationY = mTranslationFactorY * mBrushHeight;

    }
    catch (Ogre::Exception e) {}
}

//****************************************************************************/
void PaintLayer::setPaintEffect (PaintEffects paintEffect)
{
    mPaintEffect = paintEffect;
}

//****************************************************************************/
void PaintLayer::setPaintOverflow (PaintOverflowTypes paintOverflow)
{
    mPaintOverflow = paintOverflow;
}

//****************************************************************************/
void PaintLayer::setPaintColour (const Ogre::ColourValue& colourValue)
{
    mPaintColour = colourValue;
}

//****************************************************************************/
void PaintLayer::setJitterPaintColour (const Ogre::ColourValue& paintColourMin, const Ogre::ColourValue& paintColourMax)
{
    mJitterPaintColour = true;
    mJitterPaintColourMin = paintColourMin;
    mJitterPaintColourMax = paintColourMax;
}

//****************************************************************************/
void PaintLayer::setJitterPaintColourMin (const Ogre::ColourValue& paintColourMin)
{
    mJitterPaintColour = true;
    mJitterPaintColourMin = paintColourMin;
}

//****************************************************************************/
void PaintLayer::setJitterPaintColourMax (const Ogre::ColourValue& paintColourMax)
{
    mJitterPaintColour = true;
    mJitterPaintColourMax = paintColourMax;
}

//****************************************************************************/
void PaintLayer::setJitterPaintColourInterval (float interval)
{
    mJitterPaintColourInterval = interval;
}

//****************************************************************************/
void PaintLayer::resetPaintColour (void)
{
    mJitterPaintColour = false;
}

//****************************************************************************/
bool PaintLayer::hasJitterPaintEnabled (void)
{
    return mJitterPaintColour;
}

//****************************************************************************/
void PaintLayer::setJitterPaintEnabled (bool enabled)
{
    mJitterPaintColour = enabled;
}

//****************************************************************************/
void PaintLayer::setForce (float force)
{
    mForce = force;
}

//****************************************************************************/
void PaintLayer::setJitterForce (float jitterForceMin, float jitterForceMax)
{
    mJitterForce = true;
    mJitterForceMin = jitterForceMin;
    mJitterForceMax = jitterForceMax;
}

//****************************************************************************/
void PaintLayer::setJitterForceMin (float jitterForceMin)
{
    mJitterForce = true;
    mJitterForceMin = jitterForceMin;
}

//****************************************************************************/
void PaintLayer::setJitterForceMax (float jitterForceMax)
{
    mJitterForce = true;
    mJitterForceMax = jitterForceMax;
}

//****************************************************************************/
void PaintLayer::setJitterForceInterval (float interval)
{
    mJitterForceInterval = interval;
}

//****************************************************************************/
void PaintLayer::resetForce (void)
{
    mJitterForce = false;
}

//****************************************************************************/
bool PaintLayer::hasJitterForceEnabled (void)
{
    return mJitterForce;
}

//****************************************************************************/
void PaintLayer::setJitterForceEnabled (bool enabled)
{
    mJitterForce = enabled;
}

//****************************************************************************/
void PaintLayer::setRotationAngle (float rotationAngle)
{
    mRotationAngle = rotationAngle;
    mRotate = true;
    mSinRotationAngle = sin(rotationAngle * Ogre::Math::PI / 180);
    mCosRotationAngle = cos(rotationAngle * Ogre::Math::PI / 180);
}

//****************************************************************************/
void PaintLayer::setJitterRotationAngle (float rotationAngleMin, float rotationAngleMax)
{
    mRotate = true;
    mJitterRotate = true;
    mJitterRotationAngleMin = rotationAngleMin;
    mJitterRotationAngleMax = rotationAngleMax;
}

//****************************************************************************/
void PaintLayer::setJitterRotationAngleMin (float rotationAngleMin)
{
    mRotate = true;
    mJitterRotate = true;
    mJitterRotationAngleMin = rotationAngleMin;
}

//****************************************************************************/
void PaintLayer::setJitterRotationAngleMax (float rotationAngleMax)
{
    mRotate = true;
    mJitterRotate = true;
    mJitterRotationAngleMax = rotationAngleMax;
}

//****************************************************************************/
void PaintLayer::setJitterRotationAngleInterval (float interval)
{
    mJitterRotationAngleInterval = interval;
}

//****************************************************************************/
void PaintLayer::resetRotation (void)
{
    mRotate = false;
    mJitterRotate = false;
}

//****************************************************************************/
bool PaintLayer::hasJitterRotationEnabled (void)
{
    return mJitterRotate;
}

//****************************************************************************/
void PaintLayer::setJitterRotationEnabled (bool enabled)
{
    mJitterRotate = enabled;
}

//****************************************************************************/
void PaintLayer::setTranslationFactor (float translationFactorX, float translationFactorY)
{
    mTranslate = true;
    mTranslationFactorX = translationFactorX;
    mTranslationFactorY = translationFactorY;
    mTranslationX = translationFactorX * mBrushWidth;
    mTranslationY = translationFactorY * mBrushHeight;
}

//****************************************************************************/
void PaintLayer::setJitterTranslationFactor (float jitterTranslationFactorXmin,
                                             float jitterTranslationFactorXmax,
                                             float jitterTranslationFactorYmin,
                                             float jitterTranslationFactorYmax)
{
    mTranslate = true;
    mJitterTranslate = true;
    mJitterTranslationFactorXmin = jitterTranslationFactorXmin;
    mJitterTranslationFactorXmax = jitterTranslationFactorXmax;
    mJitterTranslationFactorYmin = jitterTranslationFactorYmin;
    mJitterTranslationFactorYmax = jitterTranslationFactorYmax;
}

//****************************************************************************/
void PaintLayer::setJitterTranslationFactorX (float jitterTranslationFactorXmin,
                                              float jitterTranslationFactorXmax)
{
    mTranslate = true;
    mJitterTranslate = true;
    mJitterTranslationFactorXmin = jitterTranslationFactorXmin;
    mJitterTranslationFactorXmax = jitterTranslationFactorXmax;
}

//****************************************************************************/
void PaintLayer::setJitterTranslationFactorY (float jitterTranslationFactorYmin,
                                              float jitterTranslationFactorYmax)
{
    mTranslate = true;
    mJitterTranslate = true;
    mJitterTranslationFactorYmin = jitterTranslationFactorYmin;
    mJitterTranslationFactorYmax = jitterTranslationFactorYmax;
}

//****************************************************************************/
const Ogre::Vector2& PaintLayer::getJitterTranslationFactorX (void)
{
    mHelperVector2.x = mJitterTranslationFactorXmin;
    mHelperVector2.y = mJitterTranslationFactorXmax;
    return mHelperVector2;
}

//****************************************************************************/
const Ogre::Vector2& PaintLayer::getJitterTranslationFactorY (void)
{
    mHelperVector2.x = mJitterTranslationFactorYmin;
    mHelperVector2.y = mJitterTranslationFactorYmax;
    return mHelperVector2;
}

//****************************************************************************/
void PaintLayer::setJitterTranslationInterval (float interval)
{
    mJitterTranslationInterval = interval;
}

//****************************************************************************/
void PaintLayer::resetTranslation (void)
{
    mTranslate = false;
    mJitterTranslate = false;
}

//****************************************************************************/
bool PaintLayer::hasJitterTranslationEnabled(void)
{
    return mJitterTranslate;
}

//****************************************************************************/
void PaintLayer::setJitterTranslationEnabled (bool enabled)
{
    mJitterTranslate = enabled;
}

//****************************************************************************/
void PaintLayer::setScale (float scale)
{
    mScale = scale;
    mHalfBrushWidthScaled = mScale * mHalfBrushWidth;
    mHalfBrushHeightScaled = mScale * mHalfBrushHeight;
}

//****************************************************************************/
void PaintLayer::setJitterScale (float jitterScaleMin, float jitterScaleMax)
{
    mJitterScale = true;
    mJitterScaleMax = jitterScaleMax;
    mJitterScaleMin = jitterScaleMin;
}

//****************************************************************************/
void PaintLayer::setJitterScaleMin (float jitterScaleMin)
{
    mJitterScale = true;
    mJitterScaleMin = jitterScaleMin;
}

//****************************************************************************/
void PaintLayer::setJitterScaleMax (float jitterScaleMax)
{
    mJitterScale = true;
    mJitterScaleMax = jitterScaleMax;
}

//****************************************************************************/
void PaintLayer::setJitterScaleInterval (float interval)
{
    mJitterScaleInterval = interval;
}

//****************************************************************************/
void PaintLayer::resetScale (void)
{
    mJitterScale = false;
}

//****************************************************************************/
bool PaintLayer::hasJitterScaleEnabled(void)
{
    return mJitterScale;
}

//****************************************************************************/
void PaintLayer::setJitterScaleEnabled(bool enabled)
{
    mJitterScale = enabled;
}

//****************************************************************************/
void PaintLayer::setMirrorHorizontal (bool mirrored)
{
    mMirrorHorizontal = mirrored;
}

//****************************************************************************/
void PaintLayer::setJitterMirrorHorizontal (bool enabled)
{
    mJitterMirrorHorizontal = enabled;
}

//****************************************************************************/
void PaintLayer::setJitterMirrorHorizontalInterval (float interval)
{
    mJitterMirrorHorizontalInterval = interval;
}

//****************************************************************************/
void PaintLayer::resetMirrorHorizontal (void)
{
    mMirrorHorizontal = false;
    mJitterMirrorHorizontal = false;
}

//****************************************************************************/
bool PaintLayer::hasJitterMirrorHorizontalEnabled(void)
{
    return mJitterMirrorHorizontal;
}

//****************************************************************************/
void PaintLayer::setJitterMirrorHorizontalEnabled (bool enabled)
{
    mJitterMirrorHorizontal = enabled;
}

//****************************************************************************/
void PaintLayer::setMirrorVertical (bool mirrored)
{
    mMirrorVertical = mirrored;
}

//****************************************************************************/
void PaintLayer::setJitterMirrorVertical (bool enabled)
{
    mJitterMirrorVertical = enabled;
}

//****************************************************************************/
void PaintLayer::setJitterMirrorVerticalInterval (float interval)
{
    mJitterMirrorVerticalInterval = interval;
}

//****************************************************************************/
void PaintLayer::resetMirrorVertical (void)
{
    mMirrorVertical = false;
    mJitterMirrorVertical = false;
}

//****************************************************************************/
bool PaintLayer::hasJitterMirrorVerticalEnabled(void)
{
    return mJitterMirrorVertical;
}

//****************************************************************************/
void PaintLayer::setJitterMirrorVerticalEnabled (bool enabled)
{
    mJitterMirrorVertical = enabled;
}

//****************************************************************************/
size_t PaintLayer::calculateTexturePositionX (float u, size_t brushPositionX)
{
    if (!mTextureLayer)
        return 0;

    int w = mTextureLayer->mTextureOnWhichIsPaintedWidth;
    int pos = (u * w) - mHalfBrushWidthScaled + mScale * brushPositionX;

    if (mPaintOverflow == PAINT_OVERFLOW_IGNORE)
    {
        if (pos < 0)
            pos = 0;
        else if (pos >= w)
            pos = w - 1;
        return pos;
    }
    else if (mPaintOverflow == PAINT_OVERFLOW_CONTINUE)
    {
        if (pos < 0)
            pos += w;
        else if (pos >= w)
            pos -= w;
        return pos;
    }

    return pos;
}

//****************************************************************************/
size_t PaintLayer::calculateTexturePositionY (float v, size_t brushPositionY)
{
    if (!mTextureLayer)
        return 0;

    int h = mTextureLayer->mTextureOnWhichIsPaintedHeight;
    int pos = (v * h) - mHalfBrushHeightScaled + mScale * brushPositionY;

    if (mPaintOverflow == PAINT_OVERFLOW_IGNORE)
    {
        if (pos < 0)
            pos = 0;
        else if (pos >= h)
            pos = h - 1;
        return pos;
    }
    else if (mPaintOverflow == PAINT_OVERFLOW_CONTINUE)
    {
        if (pos < 0)
            pos += h;
        else if (pos >= h)
            pos -= h;
        return pos;
    }

    return pos;
}

//****************************************************************************/
void PaintLayer::determineJitterEffects (void)
{
    // Get the elapsed time
    mEndTime = clock();
    mJitterElapsedTime = (float)(mEndTime - mStartTime) / CLOCKS_PER_SEC;
    mStartTime = mEndTime;

    // Determine whether there are jitter effects
    if (mJitterRotate)
    {
        mJitterRotateElapsedTime += mJitterElapsedTime;
        if (mJitterRotateElapsedTime > mJitterRotationAngleInterval)
        {
            setRotationAngle (randomBetweenTwoFloats(mJitterRotationAngleMin, mJitterRotationAngleMax));
            mJitterRotateElapsedTime = 0.0f;
        }
    }

    if (mJitterTranslate)
    {
        mJitterTranslationElapsedTime += mJitterElapsedTime;
        if (mJitterTranslationElapsedTime > mJitterTranslationInterval)
        {
            setTranslationFactor(randomBetweenTwoFloats(mJitterTranslationFactorXmin, mJitterTranslationFactorXmax),
                                 randomBetweenTwoFloats(mJitterTranslationFactorYmin, mJitterTranslationFactorYmax));
            mJitterTranslationElapsedTime = 0.0f;
        }
    }

    if (mJitterScale)
    {
        mJitterScaleElapsedTime += mJitterElapsedTime;
        if (mJitterScaleElapsedTime > mJitterScaleInterval)
        {
            setScale(randomBetweenTwoFloats(mJitterScaleMin, mJitterScaleMax));
            mJitterScaleElapsedTime = 0.0f;
        }
    }

    if (mJitterForce)
    {
        mJitterForceElapsedTime += mJitterElapsedTime;
        if (mJitterForceElapsedTime > mJitterForceInterval)
        {
            setForce(randomBetweenTwoFloats(mJitterForceMin, mJitterForceMax));
            mJitterForceElapsedTime = 0.0f;
        }
    }

    if (mJitterPaintColour)
    {
        mJitterPaintColourElapsedTime += mJitterElapsedTime;
        if (mJitterPaintColourElapsedTime > mJitterPaintColourInterval)
        {
            setPaintColour(randomBetweenTwoColours(mJitterPaintColourMin, mJitterPaintColourMax));
            mJitterPaintColourElapsedTime = 0.0f;
        }
    }

    if (mJitterMirrorHorizontal)
    {
        mJitterMirrorHorizontalElapsedTime += mJitterElapsedTime;
        if (mJitterMirrorHorizontalElapsedTime > mJitterMirrorHorizontalInterval)
        {
            mMirrorHorizontal = randomBool();
            mJitterMirrorHorizontalElapsedTime = 0.0f;
        }
    }

    if (mJitterMirrorVertical)
    {
        mJitterMirrorVerticalElapsedTime += mJitterElapsedTime;
        if (mJitterMirrorVerticalElapsedTime > mJitterMirrorVerticalInterval)
        {
            mMirrorVertical = randomBool();
            mJitterMirrorVerticalElapsedTime = 0.0f;
        }
    }
}

//****************************************************************************/
Ogre::IdString PaintLayer::getDatablockName (void)
{
    if (!mTextureLayer)
        return dummyDatablockId;

    return mTextureLayer->mDatablockName;
}

//****************************************************************************/
void PaintLayer::saveTextureGeneration (void)
{
    // It is never possible to save a texture when the paintlayer is not visible
    if (!mVisible)
        return;

    mTextureLayer->saveTextureGeneration ();
}
