QT += widgets opengl multimedia

OGREHOME = "C:/Users/Henry/Documents/Visual Studio 2015/Projects/ogre"


HEADERS       = \
    source/header/mainwindow.h \
    source/header/renderwindow_dockwidget.h \
    source/header/properties_dockwidget.h \
    source/header/nodeeditor_dockwidget.h \
    framework/header/asset_assetconstants.h \
    framework/header/asset_assetwidget.h \
    framework/header/asset_clickableframe.h \
    framework/header/asset_clickablelabel.h \
    framework/header/asset_containerwidget.h \
    framework/header/asset_curve_dialog.h \
    framework/header/asset_curve_grid.h \
    framework/header/asset_propertywidget.h \
    framework/header/asset_propertywidget_checkbox.h \
    framework/header/asset_propertywidget_color.h \
    framework/header/asset_propertywidget_curve.h \
    framework/header/asset_propertywidget_decimal.h \
    framework/header/asset_propertywidget_quaternion.h \
    framework/header/asset_propertywidget_select.h \
    framework/header/asset_propertywidget_slider.h \
    framework/header/asset_propertywidget_string.h \
    framework/header/asset_propertywidget_texture.h \
    framework/header/asset_propertywidget_xy.h \
    framework/header/asset_propertywidget_xyz.h \
    framework/header/magus_core.h \
    framework/header/node_compound.h \
    framework/header/node_connection.h \
    framework/header/node_constants.h \
    framework/header/node_editorwidget.h \
    framework/header/node_node.h \
    framework/header/node_port.h \
    framework/header/node_porttype.h \
    framework/header/ogre3_cameraman.h \
    framework/header/ogre3_renderman.h \
    framework/header/ogre3_widget.h \
    framework/header/ogre_prereqs.h \
    source/header/hlms_node_pbs_datablock.h \
    source/header/hlms_node_porttypes.h \
    source/header/constants.h \
    source/header/hlms_node_samplerblock.h \
    source/header/hlms_node_macroblock.h \
    source/header/hlms_node_blenddblock.h \
    source/header/hlms_properties_blenddblock.h \
    source/header/hlms_properties_macroblock.h \
    source/header/hlms_properties_pbs_datablock.h \
    source/header/hlms_properties_samplerblock.h \
    source/header/hlms_pbs_builder.h \
    source/header/hlms_builder.h \
    source/header/hlms_properties_unlit_datablock.h \
    source/header/hlms_node_unlit_datablock.h \
    source/header/hlms_unlit_builder.h \
    framework/header/tb_transformationwidget.h


SOURCES       = \
    source/src/main.cpp \
    source/src/mainwindow.cpp \
    source/src/renderwindow_dockwidget.cpp \ 
    source/src/properties_dockwidget.cpp \ 
    source/src/nodeeditor_dockwidget.cpp \ 
    framework/src/asset_assetwidget.cpp \
    framework/src/asset_containerwidget.cpp \
    framework/src/asset_curve_dialog.cpp \
    framework/src/asset_curve_grid.cpp \
    framework/src/asset_propertywidget_checkbox.cpp \
    framework/src/asset_propertywidget_color.cpp \
    framework/src/asset_propertywidget_curve.cpp \
    framework/src/asset_propertywidget_decimal.cpp \
    framework/src/asset_propertywidget_quaternion.cpp \
    framework/src/asset_propertywidget_select.cpp \
    framework/src/asset_propertywidget_slider.cpp \
    framework/src/asset_propertywidget_string.cpp \
    framework/src/asset_propertywidget_texture.cpp \
    framework/src/asset_propertywidget_xy.cpp \
    framework/src/asset_propertywidget_xyz.cpp \
    framework/src/node_compound.cpp \
    framework/src/node_connection.cpp \
    framework/src/node_editorwidget.cpp \
    framework/src/node_node.cpp \
    framework/src/node_port.cpp \
    framework/src/node_porttype.cpp \
    framework/src/ogre3_renderman.cpp \
    framework/src/ogre3_widget.cpp \
    source/src/hlms_node_pbs_datablock.cpp \
    source/src/hlms_node_porttypes.cpp \
    source/src/hlms_node_samplerblock.cpp \
    source/src/hlms_node_macroblock.cpp \
    source/src/hlms_node_blenddblock.cpp \
    source/src/hlms_properties_blenddblock.cpp \
    source/src/hlms_properties_macroblock.cpp \
    source/src/hlms_properties_pbs_datablock.cpp \
    source/src/hlms_properties_samplerblock.cpp \
    source/src/hlms_pbs_builder.cpp \
    source/src/hlms_builder.cpp \
    source/src/hlms_properties_unlit_datablock.cpp \
    source/src/hlms_node_unlit_datablock.cpp \
    source/src/hlms_unlit_builder.cpp \
    framework/src/tb_transformationwidget.cpp


INCLUDEPATH += "../HLMSEditor/source/header/"
INCLUDEPATH += "../HLMSEditor/framework/header"
INCLUDEPATH += "$$OGREHOME/OgreMain/include"
INCLUDEPATH += "$$OGREHOME/Components/Hlms/Common/include"
INCLUDEPATH += "$$OGREHOME/Components/Hlms/Pbs/include"
INCLUDEPATH += "$$OGREHOME/Components/Hlms/Unlit/include"
INCLUDEPATH += "$$OGREHOME/VCBuild/include"
INCLUDEPATH += .

Debug:LIBS += -L"$$OGREHOME/VCBuild/lib/Debug"
Release:LIBS += -L"$$OGREHOME/VCBuild/lib/Release"

CONFIG(debug, debug|release):LIBS += \
    -lopengl32 \
    -lOgreMain_d \
    -lOgreHlmsPbs_d \
    -lOgreHlmsUnlit_d

CONFIG(release, debug|release):LIBS += \
    -lopengl32 \
    -lOgreMain \
    -lOgreHlmsPbs \
    -lOgreHlmsUnlit



Release:DESTDIR = ../HLMSEditor/bin
Debug:DESTDIR = ../HLMSEditor/bin
target.path = $$[QTDIR]/
INSTALLS += target
