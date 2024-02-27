set(WIDGET_SRCS CSkip.cpp AlignWin.h AlignWin.cpp CCanvas.cpp CLabelSlider.cpp CMultiSlider.cpp CTransparentFilm.cpp CSkeleton.cpp datatype.cpp Editor.cpp Editor.h 
	TreeView.h TreeView.cpp PreprocessWin.h PreprocessWin.cpp CTransparentDemarcater.cpp CNewProject.cpp CNewProject.h 
	CSelect.h CSelect.cpp CSetWorkSpace.cpp CSetWorkSpace.h CoarseAlignWin.h CoarseAlignWin.cpp FineAlignWin.cpp  RestructWin.cpp LoadImgWin.cpp
	LoadImgWin.h CProgressBar.h CProgressBar.cpp EvaluateWin.h EvaluateWin.cpp Evaluate_tool.h Evaluate_tool.cpp 
	PostProcessWin.h PostProcessWin.cpp CTFSettingsDialog.cpp CTFSettingsDialog.h AboutDialog.cpp AboutDialog.h Ctag.cpp .)
#qt4_automoc(${WIDGET_SRCS})

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)
find_package(Qt5Widgets REQUIRED)
#find_package(Qt5Core REQUIRED)
#find_package(Qt5Gui REQUIRED)

include_directories(${QT_INCLUDES} ${CMAKE_CURRENT_BINARY_DIR}  ${PROJECT_SOURCE_DIR}/src/tomomain/ImageViewWindow ${PROJECT_SOURCE_DIR}/src/tomomain/ImageViewWindow/slicemain ${PROJECT_SOURCE_DIR}/src/tomomain/ImageViewWindow/volumemain)
add_library(widget ${WIDGET_SRCS})
target_link_libraries(widget  ${OPENCV_STATIC_LIBS} mrcimg resource  VolumeWindow ImageViewWindow env init promanage core custom dataf utils Qt5::Widgets model)

set_target_properties(widget PROPERTIES OUTPUT_NAME "widget")
set_target_properties(widget PROPERTIES CLEAN_DIRECT_OUTPUT 1)
