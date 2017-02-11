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

#ifndef PAINT_LAYER_H
#define PAINT_LAYER_H

#include "OgreRoot.h"
#include "OgreImage.h"
#include "OgreTexture.h"
#include "OgreMath.h"
#include "OgreVector2.h"
#include "OgreColourValue.h"
#include "OgreTexture.h"
#include "OgrePixelBox.h"
#include "OgreItem.h"
#include "OgreHlmsPbsDatablock.h"
#include "OgreHlmsDatablock.h"
#include "OgreHardwarePixelBuffer.h"
#include "texturelayer.h"
#include <ctime>

/****************************************************************************
 This class contains functions used by the Hlms editor painter window. A
 paintlayer represents a layer, assigned to a certain texture of a Hlms
 material. E.g. A PaintLayer can be assinged to a diffuse texture. The paint
 characteristics are set in the PaintLayer object.

 Each Hlms material can have multiple PaintLayer assigned to it. Each texture
 type (diffuse, normal, ...) can also have multiple PaintLayer assigned to it.
 Layers can be painted individually, but painting can also be done at once
 (a mouse paint stroke involves multiple PaintLayer objects).
 ***************************************************************************/

class PaintLayerManager;
class PaintLayer
{
	public:
        enum PaintEffects
        {
            PAINT_EFFECT_COLOR, /// Paint with color
            PAINT_EFFECT_ALPHA, /// Paint with alpha
            PAINT_EFFECT_TEXTURE, /// Use the colours of the brush
            PAINT_EFFECT_NORMAL_RAISED, /// Paint on a normal map and apply a raising effect; only used for normal maps
            PAINT_EFFECT_NORMAL_LOWERED /// Paint on a normal map and apply a lowering effect; only used for normal maps
        };

        enum PaintOverflowTypes
        {
            PAINT_OVERFLOW_IGNORE, /// If the brush exceeds the texture areas on which is painted, the overflow is ignored
            PAINT_OVERFLOW_TO_OPPOSITE_CORNER  /// If the brush exceeds the texture areas on which is painted, the overflow is continued on the opposite corner
        };

        PaintLayer(PaintLayerManager* paintLayerManager);
        ~PaintLayer(void);

        /* Enable or disable the layer for painting
         */
        void enable(bool enabled);

        /* Apply the paint effect.
         */
        void paint(float u, float v);

        /* Set the reference to the texture.
         */
        void setTextureLayer (TextureLayer* textureLayer);

        /* Set the name of the brush used for painting.
         */
        void setBrush (const Ogre::String& brushFileName);

        /* Set scale of the brush.
         */
        void setScale (float scale);

        /* Set jitter scale.
         * This setting creates a jittering effect in which the scale changes between two values
         */
        void setJitterScale (float jitterScaleMin, float jitterScaleMax);

        /* Determines the frequency of the jitter effect. The interval is set to seconds.
         */
        void setJitterScaleInterval (float interval);

        /* Exclude scaling the brush image
         * This also stops scale jitter.
         */
        void resetScale (void);

        /* Determine how much pressure is put on the brush. Value [0..1]
         */
        void setForce (float force);

        /* Set jitter brush force.
         * This setting creates a jittering effect in which the brush force changes between two values
         */
        void setJitterForce (float jitterForceMin, float jitterForceMax);

        /* Determines the frequency of the jitter effect. The interval is set to seconds.
         */
        void setJitterForceInterval (float interval);

        /* Stop with force jitter
         */
        void resetForce (void);

        /* Determine the rotation of the angle in degrees [0..360]
         */
        void setRotationAngle (float rotationAngle);

        /* Set jitter angle for rotating the brush.
         * This setting creates a jittering effect in which the brush rotation changes between two values
         */
        void setJitterRotationAngle (float rotationAngleMin, float rotationAngleMax);

        /* Determines the frequency of the jitter effect. The interval is set to seconds.
         */
        void setJitterRotationAngleInterval (float interval);

        /* Exclude rotating the brush image
         * This also stops rotation jitter
         */
        void resetRotation (void);

        /* Determine the translation. This is a relative value [0..1]
         */
        void setTranslation (float translationFactorX, float translationFactorY);

        /* Set jitter brush translation.
         * This setting creates a jittering effect in which the brush translation changes between two values
         */
        void setJitterTranslation (float jitterTranslationFactorXmin,
                                   float jitterTranslationFactorXmax,
                                   float jitterTranslationFactorYmin,
                                   float jitterTranslationFactorYmax);

        /* Determines the frequency of the jitter effect. The interval is set to seconds.
         */
        void setJitterTranslationInterval (float interval);

        /* Exclude translating the brush image
         * This also stops translation jitter.
         */
        void resetTranslation (void);

        /* Set the paint effect.
         */
        void setPaintEffect (PaintEffects paintEffect);

        /* Set colour used for painting.
         */
        void setPaintColour (const Ogre::ColourValue& colourValue);

        /* Set jitter colour for painting the brush.
         * This setting creates a jittering effect in which the paint colour changes between two values
         */
        void setJitterPaintColour (const Ogre::ColourValue& paintColourMin, const Ogre::ColourValue& paintColourMax);

        /* Determines the frequency of the jitter effect. The interval is set to seconds.
         */
        void setJitterPaintColourInterval (float interval);

        /* Stop with paint colour jitter
         */
        void resetPaintColour (void);

        /* Mirror the brush horizontally
         */
        void setMirrorHorizontal (bool mirrored);

        /* Set jitter mirror horizontal.
         * This setting creates an alternating mirror (horizontally) effect of the brush
         */
        void setJitterMirrorHorizontal (void);

        /* Determines the frequency of the jitter effect. The interval is set to seconds.
         */
        void setJitterMirrorHorizontalInterval (float interval);

        /* Exclude the mirror (horizontally) effect.
         * Stop the jittering effect
         */
        void resetMirrorHorizontal (void);

        /* Mirror the brush vertically
         */
        void setMirrorVertical (bool mirrored);

        /* Set jitter mirror vertical.
         * This setting creates an alternating mirror (vertically) effect of the brush
         */
        void setJitterMirrorVertical (void);

        /* Determines the frequency of the jitter effect. The interval is set to seconds.
         */
        void setJitterMirrorVerticalInterval (float interval);

        /* Exclude the mirror (vertical) effect.
         * Stop the jittering effect
         */
        void resetMirrorVertical (void);

    private:
            clock_t mStartTime;
            clock_t mEndTime;
            bool mEnabled;                                  // If enabled, the layer is painted, otherwise it is skipped
            Ogre::Image mBrush;                             // Image of the brush
            Ogre::String mBrushFileName;                    // Full qualified name of the brush file
            Ogre::PixelBox mPixelboxBrush;                  // Pixelbox of mBrush; for speed purposes, it is created when the brush is set
            size_t mBrushWidth;                             // Width of mBrush
            size_t mBrushHeight;                            // Height of mBrush
            size_t mHalfBrushWidth;                         // For efficient calculation
            size_t mHalfBrushWidthScaled;                   // For efficient calculation
            size_t mHalfBrushHeight;                        // For efficient calculation
            size_t mHalfBrushHeightScaled;                  // For efficient calculation
            size_t mBrushWidthMinusOne;                     // For efficient calculation
            size_t mBrushHeightMinusOne;                    // For efficient calculation
            float mForce;                                   // Factor that determines how must pressure is put on the brush; value between [0.0f, 1.0f].
            float mScale;                                   // Factor that scales the brush; value between [0.0f, 1.0f].
            Ogre::ColourValue mPaintColour;                 // The colour used for painting. Red, Green and Blue values are used when PAINT_EFFECT_COLOR is passed.
                                                            // Alpha value is used if PAINT_EFFECT_ALPHA is used.
            Ogre::ColourValue mFinalColour;                 // The calculated colour, based on mPaintColour and the brush, applied to the texture.
            PaintEffects mPaintEffect;                      // Type of paint effect.
            PaintOverflowTypes mPaintOverflow;              // Determine what happens if the brush exceeds the texture areas on which is painted.
            TextureLayer* mTextureLayer;                    // Reference to the texture (can be shared by other PaintLayers)
            size_t calculatedTexturePositionX;
            size_t calculatedTexturePositionY;
            float mAlpha;
            bool mRotate;                                   // If true, the brush is rotated
            float mRotationAngle;                           // The angle of rotating the brush
            float mSinRotationAngle;                        // Sin of the angle of rotating
            float mCosRotationAngle;                        // Cos of the angle of rotating
            int mPosX;
            int mPosY;
            int mPosXrotated;
            int mPosYrotated;
            bool mTranslate;                                // If true, the brush is translated
            float mTranslationFactorX;                      // The relative translation factor for X [-1..1]
            float mTranslationFactorY;                      // The relative translation factor for Y [-1..1]
            int mTranslationX;                              // The absolute X translation value in pixels; can be negative or positive
            int mTranslationY;                              // The absolute Y translation value in pixels; can be negative or positive
            bool mMirrorHorizontal;                         // Mirrors a brush horizontally
            bool mMirrorVertical;                           // Mirrors a brush vertically

            // Jitter attributes
            bool mJitterRotate;                             // If true, a jittering rotation is applied
            bool mJitterTranslate;                          // If true, a jittering translation is applied
            bool mJitterScale;                              // If true, a jittering scale is applied
            bool mJitterForce;                              // If true, a jittering brush force is applied
            bool mJitterPaintColour;                        // If true, a brush with jittering colours is applied
            bool mJitterMirrorHorizontal;                   // If true, the brush is randomly mirrored horizontally
            bool mJitterMirrorVertical;                     // If true, the brush is randomly mirrored vertically
            float mJitterRotateInterval;                    // Interval of the jitter in seconds
            float mJitterTranslationInterval;               // ,,
            float mJitterScaleInterval;                     // ,,
            float mJitterForceInterval;                     // ,,
            float mJitterPaintColourInterval;               // ,,
            float mJitterMirrorHorizontalInterval;          // ,,
            float mJitterMirrorVerticalInterval;            // ,,
            float mJitterElapsedTime;                       // Latest time probe
            float mJitterRotateElapsedTime;                 // Latest time probe for this specific effect
            float mJitterTranslationElapsedTime;              // ,,
            float mJitterScaleElapsedTime;                  // ,,
            float mJitterForceElapsedTime;                  // ,,
            float mJitterPaintColourElapsedTime;            // ,,
            float mJitterMirrorHorizontalElapsedTime;       // ,,
            float mJitterMirrorVerticalElapsedTime;         // ,,
            float mJitterRotationAngleMin;                  // Min. value used for jittering rotation
            float mJitterRotationAngleMax;                  // Max. value used for jittering rotation
            float mJitterTranslationFactorXmin;             // Min. value used for jittering translation X factor
            float mJitterTranslationFactorXmax;             // Max. value used for jittering translation X factor
            float mJitterTranslationFactorYmin;             // Min. value used for jittering translation Y factor
            float mJitterTranslationFactorYmax;             // Max. value used for jittering translation Y factor
            float mJitterScaleMin;                          // Min. value used for jittering scale
            float mJitterScaleMax;                          // Max. value used for jittering scale
            float mJitterForceMin;                          // Min. value used for jittering force
            float mJitterForceMax;                          // Max. value used for jittering force
            Ogre::ColourValue mJitterPaintColourMin;        // Min. value used for jittering paint colour
            Ogre::ColourValue mJitterPaintColourMax;        // Max. value used for jittering paint colour
            PaintLayerManager* mPaintLayerManager;

            // Private functions
            size_t calculateTexturePositionX (float u, size_t brushPositionX);
            size_t calculateTexturePositionY (float v, size_t brushPositionY);
};

typedef std::vector<PaintLayer*> PaintLayers;

#endif

