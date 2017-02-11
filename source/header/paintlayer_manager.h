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

#ifndef PAINT_LAYER_MANAGER_H
#define PAINT_LAYER_MANAGER_H

#include "paintlayer.h"
#include "texturelayer_manager.h"

typedef std::vector<PaintLayer*> PaintLayers;

/****************************************************************************
 This class manages all PaintLayers assigned to a datablock
 ***************************************************************************/
class PaintLayerManager
{
	public:
        PaintLayerManager(void);
        ~PaintLayerManager(void);

        /* Create a PaintLayer and add it to the PaintLayerManager administration.
         */
        PaintLayer* createPaintLayer (const Ogre::IdString& datablockName,
                                      Ogre::PbsTextureTypes textureType,
                                      const Ogre::String& textureFileName);

        /* Because the PaintLayer objects are managed by the PaintLayerManager,
         * they also need to be deleted by the PaintLayerManager
         */
        void removeAndDeletePaintLayer (PaintLayer* paintLayer);

        /* Because the PaintLayer objects are managed by the PaintLayerManager,
         * they also need to be deleted by the PaintLayerManager
         */
        void removeAndDeleteAllPaintLayers (void);

        /* Return the point the the PaintLayers
         */
        PaintLayers* getPaintLayers (void);

    private:
        PaintLayers mPaintLayers;
        TextureLayerManager mTextureLayerManager;
};

#endif