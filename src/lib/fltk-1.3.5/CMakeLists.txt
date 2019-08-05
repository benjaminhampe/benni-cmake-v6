cmake_minimum_required(VERSION 3.1)

project(benni-lib-fltk CXX)

if (TARGET fltk)
   message("${BoldYellow}[benni] HEADER_GUARD: ${PROJECT_NAME}${ColourReset}")
else()

   add_definitions(-DFL_LIBRARY)

   set(FLTK_SOURCE_DIR "${CMAKE_CURRENT_LIST_DIR}/src")
   set(FLTK_INCLUDE_DIR "${BENNI_SDK_INCLUDE_DIR}/FL")
   set(FLTK_HEADER_DIR "${CMAKE_CURRENT_LIST_DIR}/include/FL")

   set(FLTK_HEADERS
      ${FLTK_HEADER_DIR}/abi-version.h
      ${FLTK_HEADER_DIR}/config.h
      ${FLTK_HEADER_DIR}/dirent.h
      ${FLTK_HEADER_DIR}/Enumerations.H
      ${FLTK_HEADER_DIR}/filename.H
      ${FLTK_HEADER_DIR}/Fl_Adjuster.H
      ${FLTK_HEADER_DIR}/fl_ask.H
      ${FLTK_HEADER_DIR}/Fl_Bitmap.H
      ${FLTK_HEADER_DIR}/Fl_BMP_Image.H
      ${FLTK_HEADER_DIR}/Fl_Box.H
      ${FLTK_HEADER_DIR}/Fl_Browser_.H
      ${FLTK_HEADER_DIR}/Fl_Browser.H
      ${FLTK_HEADER_DIR}/Fl_Button.H
      ${FLTK_HEADER_DIR}/Fl_Cairo.H
      ${FLTK_HEADER_DIR}/Fl_Cairo_Window.H
      ${FLTK_HEADER_DIR}/Fl_Chart.H
      ${FLTK_HEADER_DIR}/Fl_Check_Browser.H
      ${FLTK_HEADER_DIR}/Fl_Check_Button.H
      ${FLTK_HEADER_DIR}/Fl_Choice.H
      ${FLTK_HEADER_DIR}/Fl_Clock.H
      ${FLTK_HEADER_DIR}/Fl_Color_Chooser.H
      ${FLTK_HEADER_DIR}/Fl_Copy_Surface.H
      ${FLTK_HEADER_DIR}/Fl_Counter.H
      ${FLTK_HEADER_DIR}/Fl_Device.H
      ${FLTK_HEADER_DIR}/Fl_Dial.H
      ${FLTK_HEADER_DIR}/Fl_Double_Window.H
      ${FLTK_HEADER_DIR}/fl_draw.H
      ${FLTK_HEADER_DIR}/Fl_Export.H
      ${FLTK_HEADER_DIR}/Fl_File_Browser.H
      ${FLTK_HEADER_DIR}/Fl_File_Chooser.H
      ${FLTK_HEADER_DIR}/Fl_File_Icon.H
      ${FLTK_HEADER_DIR}/Fl_File_Input.H
      ${FLTK_HEADER_DIR}/Fl_Fill_Dial.H
      ${FLTK_HEADER_DIR}/Fl_Fill_Slider.H
      ${FLTK_HEADER_DIR}/Fl_Float_Input.H
      ${FLTK_HEADER_DIR}/Fl_FormsBitmap.H
      ${FLTK_HEADER_DIR}/Fl_FormsPixmap.H
      ${FLTK_HEADER_DIR}/Fl_Free.H
      ${FLTK_HEADER_DIR}/Fl_GIF_Image.H
      ${FLTK_HEADER_DIR}/Fl_Gl_Window.H
      ${FLTK_HEADER_DIR}/Fl_Group.H
      ${FLTK_HEADER_DIR}/Fl.H
      ${FLTK_HEADER_DIR}/Fl_Help_Dialog.H
      ${FLTK_HEADER_DIR}/Fl_Help_View.H
      ${FLTK_HEADER_DIR}/Fl_Hold_Browser.H
      ${FLTK_HEADER_DIR}/Fl_Hor_Fill_Slider.H
      ${FLTK_HEADER_DIR}/Fl_Hor_Nice_Slider.H
      ${FLTK_HEADER_DIR}/Fl_Hor_Slider.H
      ${FLTK_HEADER_DIR}/Fl_Hor_Value_Slider.H
      ${FLTK_HEADER_DIR}/Fl_Image.H
      ${FLTK_HEADER_DIR}/Fl_Image_Surface.H
      ${FLTK_HEADER_DIR}/Fl_Input_Choice.H
      ${FLTK_HEADER_DIR}/Fl_Input_.H
      ${FLTK_HEADER_DIR}/Fl_Input.H
      ${FLTK_HEADER_DIR}/Fl_Int_Input.H
      ${FLTK_HEADER_DIR}/Fl_JPEG_Image.H
      ${FLTK_HEADER_DIR}/Fl_Light_Button.H
      ${FLTK_HEADER_DIR}/Fl_Line_Dial.H
      ${FLTK_HEADER_DIR}/Fl_Menu_Bar.H
      ${FLTK_HEADER_DIR}/Fl_Menu_Button.H
      ${FLTK_HEADER_DIR}/Fl_Menu_.H
      ${FLTK_HEADER_DIR}/Fl_Menu.H
      ${FLTK_HEADER_DIR}/Fl_Menu_Item.H
      ${FLTK_HEADER_DIR}/Fl_Menu_Window.H
      ${FLTK_HEADER_DIR}/fl_message.H
      ${FLTK_HEADER_DIR}/Fl_Multi_Browser.H
      ${FLTK_HEADER_DIR}/Fl_Multi_Label.H
      ${FLTK_HEADER_DIR}/Fl_Multiline_Input.H
      ${FLTK_HEADER_DIR}/Fl_Multiline_Output.H
      ${FLTK_HEADER_DIR}/Fl_Native_File_Chooser.H
      ${FLTK_HEADER_DIR}/Fl_Nice_Slider.H
      ${FLTK_HEADER_DIR}/Fl_Object.H
      ${FLTK_HEADER_DIR}/Fl_Output.H
      ${FLTK_HEADER_DIR}/Fl_Overlay_Window.H
      ${FLTK_HEADER_DIR}/Fl_Pack.H
      ${FLTK_HEADER_DIR}/Fl_Paged_Device.H
      ${FLTK_HEADER_DIR}/Fl_Pixmap.H
      ${FLTK_HEADER_DIR}/Fl_Plugin.H
      ${FLTK_HEADER_DIR}/Fl_PNG_Image.H
      ${FLTK_HEADER_DIR}/Fl_PNM_Image.H
      ${FLTK_HEADER_DIR}/Fl_Positioner.H
      ${FLTK_HEADER_DIR}/Fl_PostScript.H
      ${FLTK_HEADER_DIR}/Fl_Preferences.H
      ${FLTK_HEADER_DIR}/Fl_Printer.H
      ${FLTK_HEADER_DIR}/Fl_Progress.H
      ${FLTK_HEADER_DIR}/Fl_Radio_Button.H
      ${FLTK_HEADER_DIR}/Fl_Radio_Light_Button.H
      ${FLTK_HEADER_DIR}/Fl_Radio_Round_Button.H
      ${FLTK_HEADER_DIR}/Fl_Repeat_Button.H
      ${FLTK_HEADER_DIR}/Fl_Return_Button.H
      ${FLTK_HEADER_DIR}/Fl_RGB_Image.H
      ${FLTK_HEADER_DIR}/Fl_Roller.H
      ${FLTK_HEADER_DIR}/Fl_Round_Button.H
      ${FLTK_HEADER_DIR}/Fl_Round_Clock.H
      ${FLTK_HEADER_DIR}/Fl_Scrollbar.H
      ${FLTK_HEADER_DIR}/Fl_Scroll.H
      ${FLTK_HEADER_DIR}/Fl_Secret_Input.H
      ${FLTK_HEADER_DIR}/Fl_Select_Browser.H
      ${FLTK_HEADER_DIR}/Fl_Shared_Image.H
      ${FLTK_HEADER_DIR}/fl_show_colormap.H
      ${FLTK_HEADER_DIR}/fl_show_input.H
      ${FLTK_HEADER_DIR}/Fl_Simple_Counter.H
      ${FLTK_HEADER_DIR}/Fl_Single_Window.H
      ${FLTK_HEADER_DIR}/Fl_Slider.H
      ${FLTK_HEADER_DIR}/Fl_Spinner.H
      ${FLTK_HEADER_DIR}/Fl_Sys_Menu_Bar.H
      ${FLTK_HEADER_DIR}/Fl_Table.H
      ${FLTK_HEADER_DIR}/Fl_Table_Row.H
      ${FLTK_HEADER_DIR}/Fl_Tabs.H
      ${FLTK_HEADER_DIR}/Fl_Text_Buffer.H
      ${FLTK_HEADER_DIR}/Fl_Text_Display.H
      ${FLTK_HEADER_DIR}/Fl_Text_Editor.H
      ${FLTK_HEADER_DIR}/Fl_Tiled_Image.H
      ${FLTK_HEADER_DIR}/Fl_Tile.H
      ${FLTK_HEADER_DIR}/Fl_Timer.H
      ${FLTK_HEADER_DIR}/Fl_Toggle_Button.H
      ${FLTK_HEADER_DIR}/Fl_Toggle_Light_Button.H
      ${FLTK_HEADER_DIR}/Fl_Toggle_Round_Button.H
      ${FLTK_HEADER_DIR}/Fl_Tooltip.H
      ${FLTK_HEADER_DIR}/Fl_Tree.H
      ${FLTK_HEADER_DIR}/Fl_Tree_Item_Array.H
      ${FLTK_HEADER_DIR}/Fl_Tree_Item.H
      ${FLTK_HEADER_DIR}/Fl_Tree_Prefs.H
      ${FLTK_HEADER_DIR}/fl_types.h
      ${FLTK_HEADER_DIR}/fl_utf8.h
      ${FLTK_HEADER_DIR}/Fl_Valuator.H
      ${FLTK_HEADER_DIR}/Fl_Value_Input.H
      ${FLTK_HEADER_DIR}/Fl_Value_Output.H
      ${FLTK_HEADER_DIR}/Fl_Value_Slider.H
      ${FLTK_HEADER_DIR}/Fl_Widget.H
      ${FLTK_HEADER_DIR}/Fl_Window.H
      ${FLTK_HEADER_DIR}/Fl_Wizard.H
      ${FLTK_HEADER_DIR}/Fl_XBM_Image.H
      ${FLTK_HEADER_DIR}/Fl_XPM_Image.H
      ${FLTK_HEADER_DIR}/forms.H
      ${FLTK_HEADER_DIR}/gl2opengl.h
      ${FLTK_HEADER_DIR}/gl_draw.H
      ${FLTK_HEADER_DIR}/gl.h
      ${FLTK_HEADER_DIR}/glu.h
      ${FLTK_HEADER_DIR}/glut.H
      ${FLTK_HEADER_DIR}/mac.H
      ${FLTK_HEADER_DIR}/Makefile.in
      ${FLTK_HEADER_DIR}/math.h
      ${FLTK_HEADER_DIR}/names.h
      ${FLTK_HEADER_DIR}/platform.H
      ${FLTK_HEADER_DIR}/win32.H
      ${FLTK_HEADER_DIR}/x.H
   )

   set(FLTK_SOURCES_MAIN
      ${FLTK_SOURCE_DIR}/Fl.cxx
      ${FLTK_SOURCE_DIR}/Fl_Adjuster.cxx
      ${FLTK_SOURCE_DIR}/Fl_Bitmap.cxx
      ${FLTK_SOURCE_DIR}/Fl_Browser.cxx
      ${FLTK_SOURCE_DIR}/Fl_Browser_.cxx
      ${FLTK_SOURCE_DIR}/Fl_Browser_load.cxx
      ${FLTK_SOURCE_DIR}/Fl_Box.cxx
      ${FLTK_SOURCE_DIR}/Fl_Button.cxx
      ${FLTK_SOURCE_DIR}/Fl_Chart.cxx
      ${FLTK_SOURCE_DIR}/Fl_Check_Browser.cxx
      ${FLTK_SOURCE_DIR}/Fl_Check_Button.cxx
      ${FLTK_SOURCE_DIR}/Fl_Choice.cxx
      ${FLTK_SOURCE_DIR}/Fl_Clock.cxx
      ${FLTK_SOURCE_DIR}/Fl_Color_Chooser.cxx
      ${FLTK_SOURCE_DIR}/Fl_Copy_Surface.cxx
      ${FLTK_SOURCE_DIR}/Fl_Counter.cxx
      ${FLTK_SOURCE_DIR}/Fl_Device.cxx
      ${FLTK_SOURCE_DIR}/Fl_Dial.cxx
      ${FLTK_SOURCE_DIR}/Fl_Help_Dialog_Dox.cxx
      ${FLTK_SOURCE_DIR}/Fl_Double_Window.cxx
      ${FLTK_SOURCE_DIR}/Fl_File_Browser.cxx
      ${FLTK_SOURCE_DIR}/Fl_File_Chooser.cxx
      ${FLTK_SOURCE_DIR}/Fl_File_Chooser2.cxx
      ${FLTK_SOURCE_DIR}/Fl_File_Icon.cxx
      ${FLTK_SOURCE_DIR}/Fl_File_Input.cxx
      ${FLTK_SOURCE_DIR}/Fl_Group.cxx
      ${FLTK_SOURCE_DIR}/Fl_Help_View.cxx
      ${FLTK_SOURCE_DIR}/Fl_Image.cxx
      ${FLTK_SOURCE_DIR}/Fl_Image_Surface.cxx
      ${FLTK_SOURCE_DIR}/Fl_Input.cxx
      ${FLTK_SOURCE_DIR}/Fl_Input_.cxx
      ${FLTK_SOURCE_DIR}/Fl_Light_Button.cxx
      ${FLTK_SOURCE_DIR}/Fl_Menu.cxx
      ${FLTK_SOURCE_DIR}/Fl_Menu_.cxx
      ${FLTK_SOURCE_DIR}/Fl_Menu_Bar.cxx
      ${FLTK_SOURCE_DIR}/Fl_Menu_Button.cxx
      ${FLTK_SOURCE_DIR}/Fl_Menu_Window.cxx
      ${FLTK_SOURCE_DIR}/Fl_Menu_add.cxx
      ${FLTK_SOURCE_DIR}/Fl_Menu_global.cxx
      ${FLTK_SOURCE_DIR}/Fl_Multi_Label.cxx
      ${FLTK_SOURCE_DIR}/Fl_Native_File_Chooser.cxx
      ${FLTK_SOURCE_DIR}/Fl_Overlay_Window.cxx
      ${FLTK_SOURCE_DIR}/Fl_Pack.cxx
      ${FLTK_SOURCE_DIR}/Fl_Paged_Device.cxx
      ${FLTK_SOURCE_DIR}/Fl_Pixmap.cxx
      ${FLTK_SOURCE_DIR}/Fl_Positioner.cxx
      ${FLTK_SOURCE_DIR}/Fl_PostScript.cxx
      ${FLTK_SOURCE_DIR}/Fl_Printer.cxx
      ${FLTK_SOURCE_DIR}/Fl_Preferences.cxx
      ${FLTK_SOURCE_DIR}/Fl_Progress.cxx
      ${FLTK_SOURCE_DIR}/Fl_Repeat_Button.cxx
      ${FLTK_SOURCE_DIR}/Fl_Return_Button.cxx
      ${FLTK_SOURCE_DIR}/Fl_Roller.cxx
      ${FLTK_SOURCE_DIR}/Fl_Round_Button.cxx
      ${FLTK_SOURCE_DIR}/Fl_Scroll.cxx
      ${FLTK_SOURCE_DIR}/Fl_Scrollbar.cxx
      ${FLTK_SOURCE_DIR}/Fl_Shared_Image.cxx
      ${FLTK_SOURCE_DIR}/Fl_Single_Window.cxx
      ${FLTK_SOURCE_DIR}/Fl_Slider.cxx
      ${FLTK_SOURCE_DIR}/Fl_Table.cxx
      ${FLTK_SOURCE_DIR}/Fl_Table_Row.cxx
      ${FLTK_SOURCE_DIR}/Fl_Tabs.cxx
      ${FLTK_SOURCE_DIR}/Fl_Text_Buffer.cxx
      ${FLTK_SOURCE_DIR}/Fl_Text_Display.cxx
      ${FLTK_SOURCE_DIR}/Fl_Text_Editor.cxx
      ${FLTK_SOURCE_DIR}/Fl_Tile.cxx
      ${FLTK_SOURCE_DIR}/Fl_Tiled_Image.cxx
      ${FLTK_SOURCE_DIR}/Fl_Tooltip.cxx
      ${FLTK_SOURCE_DIR}/Fl_Tree.cxx
      ${FLTK_SOURCE_DIR}/Fl_Tree_Item_Array.cxx
      ${FLTK_SOURCE_DIR}/Fl_Tree_Item.cxx
      ${FLTK_SOURCE_DIR}/Fl_Tree_Prefs.cxx
      ${FLTK_SOURCE_DIR}/Fl_Valuator.cxx
      ${FLTK_SOURCE_DIR}/Fl_Value_Input.cxx
      ${FLTK_SOURCE_DIR}/Fl_Value_Output.cxx
      ${FLTK_SOURCE_DIR}/Fl_Value_Slider.cxx
      ${FLTK_SOURCE_DIR}/Fl_Widget.cxx
      ${FLTK_SOURCE_DIR}/Fl_Window.cxx
      ${FLTK_SOURCE_DIR}/Fl_Window_fullscreen.cxx
      ${FLTK_SOURCE_DIR}/Fl_Window_hotspot.cxx
      ${FLTK_SOURCE_DIR}/Fl_Window_iconize.cxx
      ${FLTK_SOURCE_DIR}/Fl_Window_shape.cxx
      ${FLTK_SOURCE_DIR}/Fl_Wizard.cxx
      ${FLTK_SOURCE_DIR}/Fl_XBM_Image.cxx
      ${FLTK_SOURCE_DIR}/Fl_XPM_Image.cxx
      ${FLTK_SOURCE_DIR}/Fl_abort.cxx
      ${FLTK_SOURCE_DIR}/Fl_add_idle.cxx
      ${FLTK_SOURCE_DIR}/Fl_arg.cxx
      ${FLTK_SOURCE_DIR}/Fl_compose.cxx
      ${FLTK_SOURCE_DIR}/Fl_display.cxx
      ${FLTK_SOURCE_DIR}/Fl_get_key.cxx
      ${FLTK_SOURCE_DIR}/Fl_get_system_colors.cxx
      ${FLTK_SOURCE_DIR}/Fl_grab.cxx
      ${FLTK_SOURCE_DIR}/Fl_lock.cxx
      ${FLTK_SOURCE_DIR}/Fl_own_colormap.cxx
      ${FLTK_SOURCE_DIR}/Fl_visual.cxx
      ${FLTK_SOURCE_DIR}/Fl_x.cxx
      ${FLTK_SOURCE_DIR}/filename_absolute.cxx
      ${FLTK_SOURCE_DIR}/filename_expand.cxx
      ${FLTK_SOURCE_DIR}/filename_ext.cxx
      ${FLTK_SOURCE_DIR}/filename_isdir.cxx
      ${FLTK_SOURCE_DIR}/filename_list.cxx
      ${FLTK_SOURCE_DIR}/filename_match.cxx
      ${FLTK_SOURCE_DIR}/filename_setext.cxx
      ${FLTK_SOURCE_DIR}/fl_arc.cxx
      ${FLTK_SOURCE_DIR}/fl_arci.cxx
      ${FLTK_SOURCE_DIR}/fl_ask.cxx
      ${FLTK_SOURCE_DIR}/fl_boxtype.cxx
      ${FLTK_SOURCE_DIR}/fl_color.cxx
      ${FLTK_SOURCE_DIR}/fl_cursor.cxx
      ${FLTK_SOURCE_DIR}/fl_curve.cxx
      ${FLTK_SOURCE_DIR}/fl_diamond_box.cxx
      ${FLTK_SOURCE_DIR}/fl_dnd.cxx
      ${FLTK_SOURCE_DIR}/fl_draw.cxx
      ${FLTK_SOURCE_DIR}/fl_draw_image.cxx
      ${FLTK_SOURCE_DIR}/fl_draw_pixmap.cxx
      ${FLTK_SOURCE_DIR}/fl_engraved_label.cxx
      ${FLTK_SOURCE_DIR}/fl_file_dir.cxx
      ${FLTK_SOURCE_DIR}/fl_font.cxx
      ${FLTK_SOURCE_DIR}/fl_gleam.cxx
      ${FLTK_SOURCE_DIR}/fl_gtk.cxx
      ${FLTK_SOURCE_DIR}/fl_labeltype.cxx
      ${FLTK_SOURCE_DIR}/fl_line_style.cxx
      ${FLTK_SOURCE_DIR}/fl_open_uri.cxx
      ${FLTK_SOURCE_DIR}/fl_oval_box.cxx
      ${FLTK_SOURCE_DIR}/fl_overlay.cxx
      ${FLTK_SOURCE_DIR}/fl_overlay_visual.cxx
      ${FLTK_SOURCE_DIR}/fl_plastic.cxx
      ${FLTK_SOURCE_DIR}/fl_read_image.cxx
      ${FLTK_SOURCE_DIR}/fl_rect.cxx
      ${FLTK_SOURCE_DIR}/fl_round_box.cxx
      ${FLTK_SOURCE_DIR}/fl_rounded_box.cxx
      ${FLTK_SOURCE_DIR}/fl_set_font.cxx
      ${FLTK_SOURCE_DIR}/fl_set_fonts.cxx
      ${FLTK_SOURCE_DIR}/fl_scroll_area.cxx
      ${FLTK_SOURCE_DIR}/fl_shadow_box.cxx
      ${FLTK_SOURCE_DIR}/fl_shortcut.cxx
      ${FLTK_SOURCE_DIR}/fl_show_colormap.cxx
      ${FLTK_SOURCE_DIR}/fl_symbols.cxx
      ${FLTK_SOURCE_DIR}/fl_vertex.cxx
      ${FLTK_SOURCE_DIR}/ps_image.cxx
      ${FLTK_SOURCE_DIR}/screen_xywh.cxx
      ${FLTK_SOURCE_DIR}/fl_utf8.cxx
      ${FLTK_SOURCE_DIR}/fl_encoding_latin1.cxx
      ${FLTK_SOURCE_DIR}/fl_encoding_mac_roman.cxx
      ${FLTK_SOURCE_DIR}/forms_compatability.cxx
      ${FLTK_SOURCE_DIR}/forms_bitmap.cxx
      ${FLTK_SOURCE_DIR}/forms_free.cxx
      ${FLTK_SOURCE_DIR}/forms_fselect.cxx
      ${FLTK_SOURCE_DIR}/forms_pixmap.cxx
      ${FLTK_SOURCE_DIR}/forms_timer.cxx
   )

   set(FLTK_SOURCES_FREEGLUT
      ${FLTK_SOURCE_DIR}/Fl_Gl_Choice.cxx
      ${FLTK_SOURCE_DIR}/Fl_Gl_Device_Plugin.cxx
      ${FLTK_SOURCE_DIR}/Fl_Gl_Overlay.cxx
      ${FLTK_SOURCE_DIR}/Fl_Gl_Window.cxx
      ${FLTK_SOURCE_DIR}/freeglut_geometry.cxx
      ${FLTK_SOURCE_DIR}/freeglut_stroke_mono_roman.cxx
      ${FLTK_SOURCE_DIR}/freeglut_stroke_roman.cxx
      ${FLTK_SOURCE_DIR}/freeglut_teapot.cxx
      ${FLTK_SOURCE_DIR}/gl_draw.cxx
      ${FLTK_SOURCE_DIR}/gl_start.cxx
      ${FLTK_SOURCE_DIR}/glut_compatability.cxx
      ${FLTK_SOURCE_DIR}/glut_font.cxx
   )

   set(FLTK_SOURCES_IMG
      ${FLTK_SOURCE_DIR}/fl_images_core.cxx
      ${FLTK_SOURCE_DIR}/Fl_BMP_Image.cxx
      ${FLTK_SOURCE_DIR}/Fl_File_Icon2.cxx
      ${FLTK_SOURCE_DIR}/Fl_GIF_Image.cxx
      ${FLTK_SOURCE_DIR}/Fl_Help_Dialog.cxx
      ${FLTK_SOURCE_DIR}/Fl_JPEG_Image.cxx
      ${FLTK_SOURCE_DIR}/Fl_PNG_Image.cxx
      ${FLTK_SOURCE_DIR}/Fl_PNM_Image.cxx
   )

   set(FLTK_SOURCES_C
      ${FLTK_SOURCE_DIR}/flstring.c
      ${FLTK_SOURCE_DIR}/scandir.c
      ${FLTK_SOURCE_DIR}/numericsort.c
      ${FLTK_SOURCE_DIR}/vsnprintf.c
      ${FLTK_SOURCE_DIR}/xutf8/is_right2left.c
      ${FLTK_SOURCE_DIR}/xutf8/is_spacing.c
      ${FLTK_SOURCE_DIR}/xutf8/case.c
      ${FLTK_SOURCE_DIR}/xutf8/utf8Input.c
      ${FLTK_SOURCE_DIR}/xutf8/utf8Utils.c
      ${FLTK_SOURCE_DIR}/xutf8/utf8Wrap.c
      ${FLTK_SOURCE_DIR}/xutf8/keysym2Ucs.c
      ${FLTK_SOURCE_DIR}/fl_utf.c
   )

   if (WIN32)
      list(APPEND FLTK_SOURCES_C fl_call_main.c)
   endif (WIN32)

   if (APPLE AND NOT OPTION_APPLE_X11)
      set(FLTK_SOURCES_MAC
         ${FLTK_SOURCE_DIR}/Fl_cocoa.mm
         ${FLTK_SOURCE_DIR}/Fl_Quartz_Printer.mm
         ${FLTK_SOURCE_DIR}/Fl_Native_File_Chooser_MAC.mm
         ${FLTK_SOURCE_DIR}/Fl_Sys_Menu_Bar.mm)
   else()
      set(FLTK_SOURCES_MAC)
   endif (APPLE AND NOT OPTION_APPLE_X11)

   set(FLTK_SOURCES ${FLTK_SOURCES_MAIN} ${FLTK_SOURCES_MAC} ${FLTK_SOURCES_IMG} ${FLTK_SOURCES_FREEGLUT} ${FLTK_SOURCES_C})

   #######################################################################

#	if (BENNI_SDK_SHARED)
#		message("${Red}[benni] SHARED_LIBRARY: ${PROJECT_NAME}${ColourReset}")
#		add_library(fltk SHARED "")
#	else()
      message("${Red}[benni] STATIC_LIBRARY: ${PROJECT_NAME}${ColourReset}")
      add_library(fltk STATIC "")
#	endif()

   target_include_directories(fltk PRIVATE ${FLTK_HEADER_DIR}/..)

   target_sources(fltk PRIVATE ${FLTK_SOURCES} ${FLTK_HEADERS})

   target_link_libraries(fltk PUBLIC freetype jpeg png cairo pthread z)

   if (WIN32)
      target_link_libraries(fltk PUBLIC comctl32)
   endif()

   if (UNIX)
      target_link_libraries(fltk PUBLIC Xft fontconfig Xext Xrender Xinerama Xfixes Xcursor X11)
      target_link_libraries(fltk PUBLIC dl)
   endif()

   target_include_directories(fltk PUBLIC ${FLTK_INCLUDE_DIR}) # For the SDK
   file(COPY ${FLTK_HEADERS} DESTINATION ${FLTK_INCLUDE_DIR}) # For the SDK

   #######################################################################

#	if (OPENGL_FOUND)
#	   FL_ADD_LIBRARY(fltk_gl STATIC "${GLCPPFILES}")
#	   target_link_libraries(fltk_gl fltk ${OPENGL_LIBRARIES})
#	endif (OPENGL_FOUND)


endif()

