/*
-----------------------------------------------------------------------------
This source file is part of OGRE
(Object-oriented Graphics Rendering Engine)
For the latest info, see http://www.ogre3d.org

Copyright (c) 2000-2014 Torus Knot Software Ltd

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
-----------------------------------------------------------------------------
*/

#ifndef __HLMS_EDITOR_PLUGIN_H__
#define __HLMS_EDITOR_PLUGIN_H__

#include "OgreString.h"
#include "OgreIdString.h"

static const Ogre::String GENERAL_HLMS_PLUGIN_NAME = "HlmsEditorPlugin";

namespace Ogre
{
    /* The PLUGIN_ACTION_FLAG is set by the plugin to indicate which actions the HLMS editor must perform (because
     * it cannot perform these actions itself)
     */
    enum PLUGIN_ACTION_FLAG
    {
        // **********************************************************************************************************************
        // **************************************************** GENERIC FLAGS ****************************************************
        // **********************************************************************************************************************

        // Displays a settings dialog before the import or export begins
        // Use the properties map to pass settings to the plugin
        PAF_PRE_ACTION_SETTINGS_DIALOG = 1 << 1,

        // Do not display the 'ok' message after a succesful import or export
        PAF_POST_ACTION_SUPPRESS_OK_MESSAGE = 1 << 2,

        // **********************************************************************************************************************
        // **************************************************** IMPORT FLAGS ****************************************************
        // **********************************************************************************************************************

        // The plugin indicates that the HLMS Editor must open a filedialog before the
        // executeImport() function is added. The filename (mInFileDialogName + mInFileDialogPath) is passed
        PAF_PRE_IMPORT_OPEN_FILE_DIALOG = 1 << 3,

        // Create a directory before importing
        PAF_PRE_IMPORT_MK_DIR = 1 << 4,

        // Displays a settings dialog before the import begins
        // Use the properties map to pass settings to the plugin
        PAF_PRE_IMPORT_SETTINGS_DIALOG = 1 << 5,

        // Load a project after the import (mOutReference contains the project file)
        PAF_POST_IMPORT_OPEN_PROJECT = 1 << 6,

        PAF_POST_IMPORT_SAVE_RESOURCE_LOCATIONS = 1 << 7,

        // Load a mesh after the import (mOutReference contains the mesh file)
        PAF_POST_IMPORT_LOAD_MESH = 1 << 8,

        // Load a project after the import (mOutReferencesMap contains the property to the project; use 'load_project')
        PAF_POST_IMPORT_OPEN_PROJECT_MAPREF = 1 << 9,

        // Load a mesh after the import (mOutReferencesMap contains the property to the project; use 'load_mesh')
        PAF_POST_IMPORT_LOAD_MESH_MAPREF = 1 << 10,

        // **********************************************************************************************************************
        // **************************************************** EXPORT FLAGS ****************************************************
        // **********************************************************************************************************************

        // The plugin indicates that the HLMS Editor must open a filedialog before the
        // executeExport () function is executed. The output directory (mInFileDialogPath) is passed
        PAF_PRE_EXPORT_OPEN_DIR_DIALOG = 1 << 11,

        // Delete all datablocks before the export is executed and keep the list of
        // datablocks (files) to rebuild them after the export
        PAF_PRE_EXPORT_DELETE_ALL_DATABLOCKS = 1 << 12,

        // The HLMS editor fills the mInTexturesUsedByDatablocks vector.
        // This is a performance improvement (otherwise the editor always provides the data even if not needed)
        PAF_PRE_EXPORT_TEXTURES_USED_BY_DATABLOCK = 1 << 13,

        // Displays a settings dialog before the export begins
        // Use the properties map to pass settings to the plugin
        PAF_PRE_EXPORT_SETTINGS_DIALOG = 1 << 14,

        // Delete all datablocks after the export is executed
        PAF_POST_EXPORT_DELETE_ALL_DATABLOCKS = 1 << 15
    };

    /** Class to pass data from Hlms editor to plugins */
    class HlmsEditorPluginData
    {
        public:
            // Public Hlms Editor data, exposed to the plugin
            enum PLUGIN_TYPES
            {
                UNSIGNED_SHORT,
                INT,
                FLOAT,
                STRING,
                BOOL
            };

            struct PLUGIN_PROPERTY
            {
                std::string propertyName;
                std::string labelName;
                std::string info;
                PLUGIN_TYPES type;
                bool boolValue;                      // If type is BOOL, this is the value
                float floatValue;                    // If type is FLOAT, this is the value
                float floatValueMin = 0.0f;          // If type is FLOAT, this is the minimum value
                float floatValueMax = 1.0f;          // If type is FLOAT, this is the maximum value
                unsigned short shortValue = 0;       // If type is UNSIGNED_SHORT, this is the value
                unsigned short shortValueMin = 0;    // If type is UNSIGNED_SHORT, this is the minimum value
                unsigned short shortValueMax = 1;    // If type is UNSIGNED_SHORT, this is the maximum value
                int intValue = 0;                    // If type is INT, this is the minimum value
                int intValueMin = 0;                 // If type is INT, this is the minimum value
                int intValueMax = 1;                 // If type is INT, this is the maximum value
                std::string stringValue = "";        // If type is STRING, this is the value
                unsigned int maxStringLength;        // If type is STRING, this is the maximum length
            };

            // Input (input for the plugin, output from the HLMS Editor)
            String mInProjectName; // The name of the project; without path and without extenstion
            String mInProjectPath; // The path where the projectfile, material configfile and texture configfile are stored
            String mInMaterialFileName; // The name of the material configfile
            String mInTextureFileName; // The name of the texture configfile
            String mInMeshesFileName; // The name of the meshes configfile
            String mInFileDialogName; // The full qualified name of the file selected by means of a filedialog (used for import/export)
            String mInFileDialogBaseName; // The name of the file selected by means of a filedialog (used for import/export), without path AND without extension
            String mInFileDialogPath; // The path of the file selected by means of a filedialog (used for import/export)
            String mInImportPath; // The default path used to import
            String mInExportPath; // The default path used to export
            std::vector<String> mInMaterialFileNameVector; // Vector with material names (fileName) in the material browser
            std::vector<String> mInTextureFileNameVector; // Vector with texture names (fileName) in the texture browser
            std::vector<String> mInTexturesUsedByDatablocks; // Vector with texture basenames from all the Pbs and Unlit datablocks in the material browser
            std::vector<String> mInMeshFileNames; // Vector with mesh filenames (fully qualified); in practice this may contain the current mesh
            Ogre::Item* mInItem; // The currently selected Item in the renderwidget
            Ogre::RenderWindow* mInRenderWindow; // The renderwindow of the renderwidget
            Ogre::SceneManager* mInSceneManager; // The scenemanager used in the renderwidget
            std::map<String, PLUGIN_PROPERTY> mInPropertiesMap; // Used to pass property information (e.g. from a settings/properties dialog) to the plugin

            // Input/output (data is passed two-way)

            /* Pointer to the currently editted datablock, set by the HLMS Editor; It can also be used to return a pointer
             * from the plugin to the HLMS editor (in case isImport() return true). This pointer will be used to generate
             * a node structure.
             * Note, that there is a possibility that it becomes a problem to access data from the OgreHlmsPbs / OgreHlmsUnlit components
             */
            Ogre::HlmsDatablock* mInOutCurrentDatablock;
            Ogre::IdString mInOutCurrentDatablockId;

            // Output (output from the plugin, input for the HLMS Editor)
            String mOutReference; // To be filled in by the plugin; this can be a (file) reference of the import or export
            std::map<String, PLUGIN_PROPERTY> mOutReferencesMap; // Pass multiple references (as a property map) from the plugin to the HLMS editor; these are not the properties
            String mOutSuccessText; // In case the function was executed correctly, this text can be displayed
            String mOutErrorText; // In case the function was not executed correctly, this errortext can be displayed
            std::vector<String> mOutMeshFileNames; // Vector with mesh filenames (fully qualified)

            // Constructor
            HlmsEditorPluginData (void)
            {
                mInProjectName = "";
                mInProjectPath = "";
                mInMaterialFileName = "";
                mInTextureFileName = "";
                mInMeshesFileName = "";
                mInFileDialogName = "";
                mInFileDialogBaseName = "";
                mInFileDialogPath = "";
                mInImportPath = "";
                mInExportPath = "";
                mInItem = 0;
                mInRenderWindow = 0;
                mInSceneManager = 0;
                mInMaterialFileNameVector.clear();
                mInTextureFileNameVector.clear();
                mInTexturesUsedByDatablocks.clear();
                mInPropertiesMap.clear();

                mInOutCurrentDatablockId= "";
                mInOutCurrentDatablock = 0;

                mOutErrorText = "Error while performing this function";
                mOutReference = "";
                mOutSuccessText = "";
                mOutReferencesMap.clear();
            }

            // Destructor
            virtual ~HlmsEditorPluginData (void) {}
    };

    /** Abstract class for Hlms editor plugins */
    class HlmsEditorPlugin
    {
		public:
            HlmsEditorPlugin (void) {}
            virtual ~HlmsEditorPlugin (void) {}

            // Does the plugin perform an import?
			virtual bool isImport (void) const = 0;
			
            // Does the plugin perform an export?
			virtual bool isExport (void) const = 0;

			// Return the text to be presented in the import menu
			virtual const String& getImportMenuText (void) const = 0;

			// Return the text to be presented in the export menu
			virtual const String& getExportMenuText (void) const = 0;

			// Perform the import
            virtual bool executeImport (HlmsEditorPluginData* data) = 0;

			// Perform the export
            virtual bool executeExport (HlmsEditorPluginData* data) = 0;

            // The editor must perform (additional) pre- and/or post actions
            virtual void performPreImportActions (void) = 0;
            virtual void performPostImportActions (void) = 0;

            // The editor must perform (additional) pre- and/or post actions
            virtual void performPreExportActions (void) = 0;
            virtual void performPostExportActions (void) = 0;

            // The plugin can also inform the editor to perform a pre- or post action
            virtual unsigned int getActionFlag (void) = 0;

            // The plugin can determine the properties and passes them to the HLMS editor
            // This funtion is optional and only called if PAF_PRE_ACTION_SETTINGS_DIALOG is set
            // in the getActionFlag()
            virtual std::map<std::string, HlmsEditorPluginData::PLUGIN_PROPERTY> getProperties (void) {return std::map<std::string, HlmsEditorPluginData::PLUGIN_PROPERTY>();}
    };
}

#endif
