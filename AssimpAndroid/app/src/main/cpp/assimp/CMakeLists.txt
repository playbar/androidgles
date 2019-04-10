#cmake_minimum_required(VERSION 3.4.1)
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14 -fexceptions")

add_definitions(-DVK_USE_PLATFORM_ANDROID_KHR)
add_definitions(-DASSIMP_BUILD_NO_C4D_IMPORTER)
add_definitions(-DOPENDDL_NO_USE_CPP11)
add_definitions(-DZLIB_CONST)
add_definitions(-DASSIMP_BUILD_NO_OGRE_IMPORTER)
add_definitions(-DASSIMP_BUILD_NO_OPENGEX_IMPORTER)

#get_filename_component(SHADERC_SRC
#                       ${ANDROID_NDK}/sources/third_party/shaderc
#                       ABSOLUTE)


add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/contrib/zlib)

# Import shaderc lib in NDK
#add_library(shaderc_lib STATIC IMPORTED)
#set_target_properties(shaderc_lib PROPERTIES IMPORTED_LOCATION
#                      ${SHADERC_SRC}/libs/${ANDROID_STL}/${ANDROID_ABI}/libshaderc.a)

#file(GLOB UTILS_SOURCE *.cpp)
#file(GLOB GLUE_SOURCE *.c)

#add_library(assimp SHARED
#            ${UTILS_SOURCE}
#            ${GLUE_SOURCE})

add_library(assimp SHARED
            contrib/ConvertUTF/ConvertUTF.c
            contrib/irrXML/irrXML.cpp
            contrib/unzip/ioapi.c
            contrib/unzip/unzip.c
            #contrib/openddlparser/code/OpenDDLParser.cpp
            #contrib/openddlparser/code/DDLNode.cpp
            #contrib/openddlparser/code/Value.cpp
            code/3DSConverter.cpp
            code/DefaultLogger.cpp
            code/scene.cpp
            code/Importer.cpp
            code/Version.cpp
            code/BaseProcess.cpp
            code/DefaultIOStream.cpp
            code/ValidateDataStructure.cpp
            code/BaseImporter.cpp
            code/ScenePreprocessor
            code/ProcessHelper.cpp
            code/SpatialSort.cpp
            code/XFileImporter.cpp
            code/XFileParser.cpp
            code/3DSLoader.cpp
            code/ObjFileParser.cpp
            code/ObjFileImporter.cpp
            code/AMFImporter.cpp
            code/AMFImporter_Material.cpp
            code/AMFImporter_Geometry.cpp
            code/AMFImporter_Postprocess.cpp
            code/MD3Loader.cpp
            code/MD2Loader.cpp
            code/PlyLoader.cpp
            code/MDLloader.cpp
            code/PlyParser.cpp
            code/ASELoader.cpp
            code/ASEParser.cpp
            code/HMPLoader.cpp
            code/SMDLoader.cpp
            code/MDCLoader.cpp
            code/MD5Loader.cpp
            code/MD5Parser.cpp
            code/STLLoader.cpp
            code/LWOLoader.cpp
            code/DXFLoader.cpp
            code/LWOBLoader.cpp
            code/OFFLoader.cpp
            code/ACLoader.cpp
            code/Subdivision.cpp
            code/LWOMaterial.cpp
            code/RawLoader.cpp
            code/SIBImporter.cpp
            code/StandardShapes.cpp
            code/NFFLoader.cpp
            code/BVHLoader.cpp
            code/IRRMeshLoader.cpp
            code/IRRLoader.cpp
            code/IRRShared.cpp
            code/Q3DLoader.cpp
            code/CSMLoader.cpp
            code/UnrealLoader.cpp
            code/LWSLoader.cpp
            code/ColladaLoader.cpp
            code/TerragenLoader.cpp
            code/SkeletonMeshBuilder.cpp
            code/MDLMaterialLoader.cpp
            code/RemoveComments.cpp
            code/SceneCombiner.cpp
            code/ObjFileMtlImporter.cpp
            code/SGSpatialSort.cpp
            code/ColladaParser.cpp
            code/CreateAnimMesh.cpp
            code/B3DImporter.cpp
            code/LWOAnimation.cpp
            #code/OpenGEXImporter.cpp
            code/MS3DLoader.cpp
            code/COBLoader.cpp
            #code/OgreImporter.cpp
            #code/OgreBinarySerializer.cpp
            #code/OgreStructs.cpp
            #code/OgreXmlSerializer.cpp
            #code/OgreMaterial.cpp
            code/ConvertToLHProcess.cpp
            code/glTFImporter.cpp
            code/AssbinLoader.cpp
            code/XGLLoader.cpp
            #code/BlenderLoader.cpp
            #code/IFCLoader.cpp
            code/FBXAnimation.cpp
            code/FBXBinaryTokenizer.cpp
            code/FBXConverter.cpp
            code/FBXDeformer.cpp
            code/FBXDocument.cpp
            code/FBXDocumentUtil.cpp
            code/FBXImporter.cpp
            code/FBXMaterial.cpp
            code/FBXMeshGeometry.cpp
            code/FBXModel.cpp
            code/FBXNodeAttribute.cpp
            code/FBXParser.cpp
            code/FBXProperties.cpp
            code/FBXTokenizer.cpp
            code/FBXUtil.cpp
            code/D3MFImporter.cpp
            code/D3MFOpcPackage.cpp
            code/MakeVerboseFormat.cpp
            code/X3DImporter.cpp
            code/X3DImporter_Geometry2D.cpp
            code/X3DImporter_Geometry3D.cpp
            code/X3DImporter_Group.cpp
            code/X3DImporter_Light.cpp
            code/X3DImporter_Metadata.cpp
            code/X3DImporter_Networking.cpp
            code/X3DImporter_Postprocess.cpp
            code/X3DImporter_Rendering.cpp
            code/X3DImporter_Shape.cpp
            code/X3DImporter_Texturing.cpp
            code/ImporterRegistry.cpp  #
            code/PostStepRegistry.cpp  #
            code/ConvertToLHProcess.cpp
            code/RemoveVCProcess.cpp
            code/RemoveRedundantMaterials.cpp
            code/FindInstancesProcess.cpp
            code/OptimizeGraph.cpp
            code/FindDegenerates.cpp
            code/ComputeUVMappingProcess.cpp
            code/PretransformVertices.cpp
            code/TriangulateProcess.cpp
            code/TextureTransform.cpp
            code/SortByPTypeProcess.cpp
            code/FindInvalidDataProcess.cpp
            code/OptimizeMeshes.cpp
            code/FixNormalsStep.cpp
            code/SplitByBoneCountProcess.cpp
            code/SplitLargeMeshes.cpp
            code/GenFaceNormalsProcess.cpp
            code/GenVertexNormalsProcess.cpp
            code/CalcTangentsProcess.cpp
            code/JoinVerticesProcess.cpp
            code/SplitLargeMeshes.cpp
            code/DeboneProcess.cpp
            code/LimitBoneWeightsProcess.cpp
            code/ImproveCacheLocality.cpp
            code/VertexTriangleAdjacency.cpp
            code/DefaultIOSystem.cpp
            code/MaterialSystem.cpp)

target_include_directories(assimp PRIVATE
                          ${CMAKE_CURRENT_SOURCE_DIR}
                          ${CMAKE_CURRENT_SOURCE_DIR}/include
                          ${CMAKE_CURRENT_SOURCE_DIR}/contrib
                          ${CMAKE_CURRENT_SOURCE_DIR}/contrib/irrXML
                          ${CMAKE_CURRENT_SOURCE_DIR}/contrib/unzip
                          ${CMAKE_CURRENT_SOURCE_DIR}/contrib/openddlparser/include
                          ${CMAKE_CURRENT_SOURCE_DIR}/contrib/rapidjson/include
                          ${CMAKE_CURRENT_SOURCE_DIR}/contrib/ConvertUTF)

target_link_libraries(assimp
                      android
                      zlibstatic
                      log )
