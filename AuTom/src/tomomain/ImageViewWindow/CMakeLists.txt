
add_subdirectory(algorithm)
add_subdirectory(3drender)
add_subdirectory(abstract)
add_subdirectory(model)
add_subdirectory(widgets)
add_subdirectory(slicemain)
add_subdirectory(volumemain)

set(IMAGEVW_DIR ${PROJECT_SOURCE_DIR}/src/tomomain/ImageViewWindow)
include_directories( ${IMAGEVW_DIR} ${IMAGEVW_DIR}/widgets ${IMAGEVW_DIR}/model ${IMAGEVW_DIR}/3drender ${IMAGEVW_DIR}/3drender/geometry ${IMAGEVW_DIR}/3drender/shader ${IMAGEVW_DIR}/algorithm  ${IMAGEVW_DIR}/abstract )
