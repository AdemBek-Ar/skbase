//  _____ __   ___ ____    ___ ____ ____      __       __
// |  ___|  |_/  /| __ |  |_  |  __| _  |  __|  |__ __|  |__  SoftwareKitBase for C++26
// |___  |   _  | | __  || __ |__  |  __| |__    __|__    __| version 0.9.5
// |_____|__| \__\|_____||____|____|_____|   |__|     |__|    https://github.com/AdamBekj/SKBase++
//
// SPDX-FileCopyrightText: 2025 - 2030 Adam Bekoudj <https://AdamBekj.me>
// SPDX-License-Identifier: MIT

/****************************************************************************\
 * this library is just a tools kit that c++ should add to std library      *
 * that virsion is the first uploaded version                               *
 * no documentation or README.md for the library yet                        *
\****************************************************************************/
export module skbase;

export import skbase.base;
import skbase.json;
import skbase.geomath;
import skbase.graph;
import skbase.files;
import skbase.thread;
import skbase.extention;
import skbase.input;
import skbase.lang;
import skbase.ui;
import std;

using namespace geometry;

export namespace nlm {
	using json_t = json;
}

export namespace ucc {
	/*template<bool auto_dbgm=false>
	using ucompiler_t = Language::Compiler<auto_dbgm>;
	using gcc_ast_t = Language::gcc_tree_t;
	using parser_t  = Language::Parser;*/
	template<bool auto_dbg=false>
	using lexer_t   = Language::Lexer<auto_dbg>;
	using kwm_t     = Language::Keywords;
	using token_t   = Language::Token;



	template<typename item_t> using arr_t= Arrays::Array<item_t>;



	using keystate_t = Input::KeyStates;
	using joystick_t = Input::Joystick;
	using mouse_t    = Input::Mouse;
	using kb_t       = UI::KeyBoard;


	
	template <typename num_t=float, std::size_t Size=3> requires ((Size == 2 or Size == 3) and std::is_arithmetic_v<num_t>) using cam_t = graph::Camera<num_t, Size>;
	template <typename num_t=float, std::size_t Size=3> requires ((Size == 2 or Size == 3) and std::is_arithmetic_v<num_t>) using lamp_t= graph::Lamp  <num_t, Size>;
	template <typename num_t> requires std::is_arithmetic_v<num_t> using cam3d_t = graph::Camera<num_t, 3>;
	template <typename num_t> requires std::is_arithmetic_v<num_t> using cam2d_t = graph::Camera<num_t, 2>;
	template <typename num_t> requires std::is_arithmetic_v<num_t> using lamp3d_t= graph::Lamp  <num_t, 3>;
	template <typename num_t> requires std::is_arithmetic_v<num_t> using lamp2d_t= graph::Lamp  <num_t, 2>;
	using cam3df_t = graph::Camera<float, 3>;
	using cam2df_t = graph::Camera<float, 2>;
	using lamp3df_t= graph::Lamp  <float, 3>;
	using lamp2df_t= graph::Lamp  <float, 2>;
	using shader_t = graph::Shader;
	using graph_t  = graph::Graph;
	using graphwere_t   = graph::GraphicEngine;
	using graphsetting_t= graph::BasicGraph;
	using gpu_graph_t   = graph::GPUGraph;
	using cpu_graph_t   = graph::CPUGraph;
//	using apu_graph_t   = graph::APUGraph;



//	using audio_t       = Audio::AudioPlayer;
//	using sound_t       = Audio::AudioPlayer;
//	using radio_wave_t  = Audio::WavHeader;
//	using wave_header_t = Audio::WavHeader;
//	using wave_t        = Audio::Wave_Info;
//	using radio_t       = Audio::Radio;



//	using wtxt_v_t= str::WStringView;
//	using txt_v_t = str::StringView;
	using wtxt_t  = str::WString;
	using txt_t   = str::String;



//	using cmprfile_t = file::CompressedFile;
	using folder_t   = file::Folder;
	using file_t     = file::File;



	using counter_t = Threads::Counter;
	using time_t    = Threads::Timer;



	using wind_t  = UI::Window;
	namespace ui {
		using drop_h_t= UI::DropHead;
		using check_t = UI::CheckBox;
		using btn_t   = UI::Button;
		using label_t = UI::Label;
		using combo_t = UI::Combo;
		using panel_t = UI::Panel;
		using item_t  = UI::UItem;
	}



	template<typename num_t, std::size_t s, bool c=false, bool n=false, bool t=false>
	requires std::is_arithmetic_v<num_t>
	using point_t = geometry::Point<num_t, s, c, n, t>;

	template<typename num_t > requires std::is_arithmetic_v<num_t> using p2d_t   = geometry::Point<num_t, 2, false, false, false>;
	template<typename num_t > requires std::is_arithmetic_v<num_t> using p3d_t   = geometry::Point<num_t, 3, false, false, false>;
	template<typename num_t > requires std::is_arithmetic_v<num_t> using p4d_t   = geometry::Point<num_t, 4, false, false, false>;
	template<typename num_t > requires std::is_arithmetic_v<num_t> using c3_t    = geometry::Point<num_t, 3, false, false, false>;
	template<typename num_t > requires std::is_arithmetic_v<num_t> using c4_t    = geometry::Point<num_t, 4, false, false, false>;
	template<typename num_t > requires std::is_arithmetic_v<num_t> using p2d_t_t = geometry::Point<num_t, 2, false, false, true>;
	template<typename num_t > requires std::is_arithmetic_v<num_t> using p3d_t_t = geometry::Point<num_t, 3, false, false, true>;
	template<typename num_t > requires std::is_arithmetic_v<num_t> using p4d_t_t = geometry::Point<num_t, 4, false, false, true>;
	template<typename num_t > requires std::is_arithmetic_v<num_t> using p2d_n_t = geometry::Point<num_t, 2, false, true, false>;
	template<typename num_t > requires std::is_arithmetic_v<num_t> using p3d_n_t = geometry::Point<num_t, 3, false, true, false>;
	template<typename num_t > requires std::is_arithmetic_v<num_t> using p4d_n_t = geometry::Point<num_t, 4, false, true, false>;
	template<typename num_t > requires std::is_arithmetic_v<num_t> using p2d_c_t = geometry::Point<num_t, 2, true, false, false>;
	template<typename num_t > requires std::is_arithmetic_v<num_t> using p3d_c_t = geometry::Point<num_t, 3, true, false, false>;
	template<typename num_t > requires std::is_arithmetic_v<num_t> using p4d_c_t = geometry::Point<num_t, 4, true, false, false>;
	template<typename num_t > requires std::is_arithmetic_v<num_t> using p2d_nt_t= geometry::Point<num_t, 2, false, true, true>;
	template<typename num_t > requires std::is_arithmetic_v<num_t> using p3d_nt_t= geometry::Point<num_t, 3, false, true, true>;
	template<typename num_t > requires std::is_arithmetic_v<num_t> using p4d_nt_t= geometry::Point<num_t, 4, false, true, true>;
	template<typename num_t > requires std::is_arithmetic_v<num_t> using p2d_cn_t= geometry::Point<num_t, 2, true, true, false>;
	template<typename num_t > requires std::is_arithmetic_v<num_t> using p3d_cn_t= geometry::Point<num_t, 3, true, true, false>;
	template<typename num_t > requires std::is_arithmetic_v<num_t> using p4d_cn_t= geometry::Point<num_t, 4, true, true, false>;
	template<typename num_t > requires std::is_arithmetic_v<num_t> using p2d_ct_t= geometry::Point<num_t, 2, true, false, true>;
	template<typename num_t > requires std::is_arithmetic_v<num_t> using p3d_ct_t= geometry::Point<num_t, 3, true, false, true>;
	template<typename num_t > requires std::is_arithmetic_v<num_t> using p4d_ct_t= geometry::Point<num_t, 4, true, false, true>;
	template<typename num_t > requires std::is_arithmetic_v<num_t> using v2d_t   = geometry::Point<num_t, 2, true, true, true>;
	template<typename num_t > requires std::is_arithmetic_v<num_t> using v3d_t   = geometry::Point<num_t, 3, true, true, true>;
	template<typename num_t > requires std::is_arithmetic_v<num_t> using v4d_t   = geometry::Point<num_t, 4, true, true, true>;
	using p2df_t   = geometry::Point<float, 2, false, false, false>;
	using p3df_t   = geometry::Point<float, 3, false, false, false>;
	using p4df_t   = geometry::Point<float, 4, false, false, false>;
	using c3f_t    = geometry::Point<float, 3, false, false, false>;
	using c4f_t    = geometry::Point<float, 4, false, false, false>;
	using p2df_t_t = geometry::Point<float, 2, false, false, true>;
	using p3df_t_t = geometry::Point<float, 3, false, false, true>;
	using p4df_t_t = geometry::Point<float, 4, false, false, true>;
	using p2df_n_t = geometry::Point<float, 2, false, true, false>;
	using p3df_n_t = geometry::Point<float, 3, false, true, false>;
	using p4df_n_t = geometry::Point<float, 4, false, true, false>;
	using p2df_c_t = geometry::Point<float, 2, true, false, false>;
	using p3df_c_t = geometry::Point<float, 3, true, false, false>;
	using p4df_c_t = geometry::Point<float, 4, true, false, false>;
	using p2df_nt_t= geometry::Point<float, 2, false, true, true>;
	using p3df_nt_t= geometry::Point<float, 3, false, true, true>;
	using p4df_nt_t= geometry::Point<float, 4, false, true, true>;
	using p2df_cn_t= geometry::Point<float, 2, true, true, false>;
	using p3df_cn_t= geometry::Point<float, 3, true, true, false>;
	using p4df_cn_t= geometry::Point<float, 4, true, true, false>;
	using p2df_ct_t= geometry::Point<float, 2, true, false, true>;
	using p3df_ct_t= geometry::Point<float, 3, true, false, true>;
	using p4df_ct_t= geometry::Point<float, 4, true, false, true>;
	using v2df_t   = geometry::Point<float, 2, true, true, true>;
	using v3df_t   = geometry::Point<float, 3, true, true, true>;
	using v4df_t   = geometry::Point<float, 4, true, true, true>;



	template<typename num_t > requires std::is_arithmetic_v<num_t> using r2d_t   = geometry::Line<num_t, 2, false, false, false>;
	template<typename num_t > requires std::is_arithmetic_v<num_t> using r3d_t   = geometry::Line<num_t, 3, false, false, false>;
	template<typename num_t > requires std::is_arithmetic_v<num_t> using r4d_t   = geometry::Line<num_t, 4, false, false, false>;
	template<typename num_t > requires std::is_arithmetic_v<num_t> using l2d_t   = geometry::Line<num_t, 2, false, false, false>;
	template<typename num_t > requires std::is_arithmetic_v<num_t> using l3d_t   = geometry::Line<num_t, 3, false, false, false>;
	template<typename num_t > requires std::is_arithmetic_v<num_t> using l4d_t   = geometry::Line<num_t, 4, false, false, false>;
	template<typename num_t > requires std::is_arithmetic_v<num_t> using l2d_t_t = geometry::Line<num_t, 2, false, false, true>;
	template<typename num_t > requires std::is_arithmetic_v<num_t> using l3d_t_t = geometry::Line<num_t, 3, false, false, true>;
	template<typename num_t > requires std::is_arithmetic_v<num_t> using l2d_n_t = geometry::Line<num_t, 2, false, true, false>;
	template<typename num_t > requires std::is_arithmetic_v<num_t> using l3d_n_t = geometry::Line<num_t, 3, false, true, false>;
	template<typename num_t > requires std::is_arithmetic_v<num_t> using l2d_c_t = geometry::Line<num_t, 2, true, false, false>;
	template<typename num_t > requires std::is_arithmetic_v<num_t> using l3d_c_t = geometry::Line<num_t, 3, true, false, false>;
	template<typename num_t > requires std::is_arithmetic_v<num_t> using l2d_nt_t= geometry::Line<num_t, 2, false, true, true>;
	template<typename num_t > requires std::is_arithmetic_v<num_t> using l3d_nt_t= geometry::Line<num_t, 3, false, true, true>;
	template<typename num_t > requires std::is_arithmetic_v<num_t> using l2d_cn_t= geometry::Line<num_t, 2, true, true, false>;
	template<typename num_t > requires std::is_arithmetic_v<num_t> using l3d_cn_t= geometry::Line<num_t, 3, true, true, false>;
	template<typename num_t > requires std::is_arithmetic_v<num_t> using l2d_ct_t= geometry::Line<num_t, 2, true, false, true>;
	template<typename num_t > requires std::is_arithmetic_v<num_t> using l3d_ct_t= geometry::Line<num_t, 3, true, false, true>;
	template<typename num_t > requires std::is_arithmetic_v<num_t> using vl2d_t  = geometry::Line<num_t, 2, true, true, true>;
	template<typename num_t > requires std::is_arithmetic_v<num_t> using vl3d_t  = geometry::Line<num_t, 3, true, true, true>;
	using r2df_t   = geometry::Line<float, 2, false, false, false>;
	using r3df_t   = geometry::Line<float, 3, false, false, false>;
	using r4df_t   = geometry::Line<float, 4, false, false, false>;
	using l2df_t   = geometry::Line<float, 2, false, false, false>;
	using l3df_t   = geometry::Line<float, 3, false, false, false>;
	using l4df_t   = geometry::Line<float, 4, false, false, false>;
	using l2df_t_t = geometry::Line<float, 2, false, false, true>;
	using l3df_t_t = geometry::Line<float, 3, false, false, true>;
	using l2df_n_t = geometry::Line<float, 2, false, true, false>;
	using l3df_n_t = geometry::Line<float, 3, false, true, false>;
	using l2df_c_t = geometry::Line<float, 2, true, false, false>;
	using l3df_c_t = geometry::Line<float, 3, true, false, false>;
	using l2df_nt_t= geometry::Line<float, 2, false, true, true>;
	using l3df_nt_t= geometry::Line<float, 3, false, true, true>;
	using l2df_cn_t= geometry::Line<float, 2, true, true, false>;
	using l3df_cn_t= geometry::Line<float, 3, true, true, false>;
	using l2df_ct_t= geometry::Line<float, 2, true, false, true>;
	using l3df_ct_t= geometry::Line<float, 3, true, false, true>;
	using vl2df_t  = geometry::Line<float, 2, true, true, true>;
	using vl3df_t  = geometry::Line<float, 3, true, true, true>;

/* name tabel
	          d c n t i
	#face     . ! ! ! !
	#clrd     . * ! ! !
	#poly     . ! * ! !
	#txr      . ! ! * !
	#shp      . ! ! ! *

	#cshp     . * ! ! *
	#cplx     . ! * ! *
	#ctxr     . * ! * !
	#cpoly    . * * ! !
	#tpoly    . ! * * !
	#tshp     . ! ! * *

	#tcplx    . ! * * *
	#wllp     . * ! * *
	#model    . * * ! *
	#fface    . * * * !
	#mesh     . * * * */

	template<typename num_t> using f2d_t     = geometry::Face<num_t, 2, false, false, false, false>;
	template<typename num_t> using f3d_t     = geometry::Face<num_t, 3, false, false, false, false>;
	template<typename num_t> using f4d_t     = geometry::Face<num_t, 4, false, false, false, false>;
	template<typename num_t> using txr2d_t   = geometry::Face<num_t, 2, false, false, true , false>;
	template<typename num_t> using txr3d_t   = geometry::Face<num_t, 3, false, false, true , false>;
	template<typename num_t> using txr4d_t   = geometry::Face<num_t, 4, false, false, true , false>;
	template<typename num_t> using clrd2d_t  = geometry::Face<num_t, 2, true , false, false, false>;
	template<typename num_t> using clrd3d_t  = geometry::Face<num_t, 3, true , false, false, false>;
	template<typename num_t> using clrd4d_t  = geometry::Face<num_t, 4, true , false, false, false>;
	template<typename num_t> using ctxr2d_t  = geometry::Face<num_t, 2, true , false, true , false>;
	template<typename num_t> using ctxr3d_t  = geometry::Face<num_t, 3, true , false, true , false>;
	template<typename num_t> using ctxr4d_t  = geometry::Face<num_t, 4, true , false, true , false>;
	template<typename num_t> using poly2d_t  = geometry::Face<num_t, 2, false, true , false, false>;
	template<typename num_t> using poly3d_t  = geometry::Face<num_t, 3, false, true , false, false>;
	template<typename num_t> using poly4d_t  = geometry::Face<num_t, 4, false, true , false, false>;
	template<typename num_t> using cpoly2d_t = geometry::Face<num_t, 2, true , true , false, false>;
	template<typename num_t> using cpoly3d_t = geometry::Face<num_t, 3, true , true , false, false>;
	template<typename num_t> using cpoly4d_t = geometry::Face<num_t, 4, true , true , false, false>;
	template<typename num_t> using tpoly2d_t = geometry::Face<num_t, 2, false, true , true , false>;
	template<typename num_t> using tpoly3d_t = geometry::Face<num_t, 3, false, true , true , false>;
	template<typename num_t> using tpoly4d_t = geometry::Face<num_t, 4, false, true , true , false>;
	template<typename num_t> using shp2d_t   = geometry::Face<num_t, 2, false, false, false, true >;
	template<typename num_t> using shp3d_t   = geometry::Face<num_t, 3, false, false, false, true >;
	template<typename num_t> using shp4d_t   = geometry::Face<num_t, 4, false, false, false, true >;
	template<typename num_t> using cshp2d_t  = geometry::Face<num_t, 2, true , false, false, true >;
	template<typename num_t> using cshp3d_t  = geometry::Face<num_t, 3, true , false, false, true >;
	template<typename num_t> using cshp4d_t  = geometry::Face<num_t, 4, true , false, false, true >;
	template<typename num_t> using tshp2d_t  = geometry::Face<num_t, 2, false, false, true , true >;
	template<typename num_t> using tshp3d_t  = geometry::Face<num_t, 3, false, false, true , true >;
	template<typename num_t> using tshp4d_t  = geometry::Face<num_t, 4, false, false, true , true >;
	template<typename num_t> using cplx2d_t  = geometry::Face<num_t, 2, false, true , false, true >;
	template<typename num_t> using cplx3d_t  = geometry::Face<num_t, 3, false, true , false, true >;
	template<typename num_t> using cplx4d_t  = geometry::Face<num_t, 4, false, true , false, true >;
	template<typename num_t> using tcplx2d_t = geometry::Face<num_t, 2, false, true , true , true >;
	template<typename num_t> using tcplx3d_t = geometry::Face<num_t, 3, false, true , true , true >;
	template<typename num_t> using tcplx4d_t = geometry::Face<num_t, 4, false, true , true , true >;
	template<typename num_t> using wllp2d_t  = geometry::Face<num_t, 2, true , false, true , true >;
	template<typename num_t> using wllp3d_t  = geometry::Face<num_t, 3, true , false, true , true >;
	template<typename num_t> using wllp4d_t  = geometry::Face<num_t, 4, true , false, true , true >;
	template<typename num_t> using model2d_t = geometry::Face<num_t, 2, true , true , false, true >;
	template<typename num_t> using model3d_t = geometry::Face<num_t, 3, true , true , false, true >;
	template<typename num_t> using model4d_t = geometry::Face<num_t, 4, true , true , false, true >;
	template<typename num_t> using fface2d_t = geometry::Face<num_t, 2, true , true , true , false>;
	template<typename num_t> using fface3d_t = geometry::Face<num_t, 3, true , true , true , false>;
	template<typename num_t> using fface4d_t = geometry::Face<num_t, 4, true , true , true , false>;
	template<typename num_t> using mesh2d_t  = geometry::Face<num_t, 2, true , true , true , true >;
	template<typename num_t> using mesh3d_t  = geometry::Face<num_t, 3, true , true , true , true >;
	template<typename num_t> using mesh4d_t  = geometry::Face<num_t, 4, true , true , true , true >;
	using f2df_t     = geometry::Face<float, 2, false, false, false, false>;
	using f3df_t     = geometry::Face<float, 3, false, false, false, false>;
	using f4df_t     = geometry::Face<float, 4, false, false, false, false>;
	using txr2df_t   = geometry::Face<float, 2, false, false, true , false>;
	using txr3df_t   = geometry::Face<float, 3, false, false, true , false>;
	using txr4df_t   = geometry::Face<float, 4, false, false, true , false>;
	using clrd2df_t  = geometry::Face<float, 2, true , false, false, false>;
	using clrd3df_t  = geometry::Face<float, 3, true , false, false, false>;
	using clrd4df_t  = geometry::Face<float, 4, true , false, false, false>;
	using ctxr2df_t  = geometry::Face<float, 2, true , false, true , false>;
	using ctxr3df_t  = geometry::Face<float, 3, true , false, true , false>;
	using ctxr4df_t  = geometry::Face<float, 4, true , false, true , false>;
	using poly2df_t  = geometry::Face<float, 2, false, true , false, false>;
	using poly3df_t  = geometry::Face<float, 3, false, true , false, false>;
	using poly4df_t  = geometry::Face<float, 4, false, true , false, false>;
	using cpoly2df_t = geometry::Face<float, 2, true , true , false, false>;
	using cpoly3df_t = geometry::Face<float, 3, true , true , false, false>;
	using cpoly4df_t = geometry::Face<float, 4, true , true , false, false>;
	using tpoly2df_t = geometry::Face<float, 2, false, true , true , false>;
	using tpoly3df_t = geometry::Face<float, 3, false, true , true , false>;
	using tpoly4df_t = geometry::Face<float, 4, false, true , true , false>;
	using shp2df_t   = geometry::Face<float, 2, false, false, false, true >;
	using shp3df_t   = geometry::Face<float, 3, false, false, false, true >;
	using shp4df_t   = geometry::Face<float, 4, false, false, false, true >;
	using cshp2df_t  = geometry::Face<float, 2, true , false, false, true >;
	using cshp3df_t  = geometry::Face<float, 3, true , false, false, true >;
	using cshp4df_t  = geometry::Face<float, 4, true , false, false, true >;
	using tshp2df_t  = geometry::Face<float, 2, false, false, true , true >;
	using tshp3df_t  = geometry::Face<float, 3, false, false, true , true >;
	using tshp4df_t  = geometry::Face<float, 4, false, false, true , true >;
	using cplx2df_t  = geometry::Face<float, 2, false, true , false, true >;
	using cplx3df_t  = geometry::Face<float, 3, false, true , false, true >;
	using cplx4df_t  = geometry::Face<float, 4, false, true , false, true >;
	using tcplx2df_t = geometry::Face<float, 2, false, true , true , true >;
	using tcplx3df_t = geometry::Face<float, 3, false, true , true , true >;
	using tcplx4df_t = geometry::Face<float, 4, false, true , true , true >;
	using wllp2df_t  = geometry::Face<float, 2, true , false, true , true >;
	using wllp3df_t  = geometry::Face<float, 3, true , false, true , true >;
	using wllp4df_t  = geometry::Face<float, 4, true , false, true , true >;
	using model2df_t = geometry::Face<float, 2, true , true , false, true >;
	using model3df_t = geometry::Face<float, 3, true , true , false, true >;
	using model4df_t = geometry::Face<float, 4, true , true , false, true >;
	using fface2df_t = geometry::Face<float, 2, true , true , true , false>;
	using fface3df_t = geometry::Face<float, 3, true , true , true , false>;
	using fface4df_t = geometry::Face<float, 4, true , true , true , false>;
	using mesh2df_t  = geometry::Face<float, 2, true , true , true , true >;
	using mesh3df_t  = geometry::Face<float, 3, true , true , true , true >;
	using mesh4df_t  = geometry::Face<float, 4, true , true , true , true >;



	template<typename num_t> using f2dl_t     = graph::ShapeLinker<num_t, 2, false, false, false, false>;
	template<typename num_t> using f3dl_t     = graph::ShapeLinker<num_t, 3, false, false, false, false>;
	template<typename num_t> using f4dl_t     = graph::ShapeLinker<num_t, 4, false, false, false, false>;
	template<typename num_t> using txr2dl_t   = graph::ShapeLinker<num_t, 2, false, false, true , false>;
	template<typename num_t> using txr3dl_t   = graph::ShapeLinker<num_t, 3, false, false, true , false>;
	template<typename num_t> using txr4dl_t   = graph::ShapeLinker<num_t, 4, false, false, true , false>;
	template<typename num_t> using clrd2dl_t  = graph::ShapeLinker<num_t, 2, true , false, false, false>;
	template<typename num_t> using clrd3dl_t  = graph::ShapeLinker<num_t, 3, true , false, false, false>;
	template<typename num_t> using clrd4dl_t  = graph::ShapeLinker<num_t, 4, true , false, false, false>;
	template<typename num_t> using ctxr2dl_t  = graph::ShapeLinker<num_t, 2, true , false, true , false>;
	template<typename num_t> using ctxr3dl_t  = graph::ShapeLinker<num_t, 3, true , false, true , false>;
	template<typename num_t> using ctxr4dl_t  = graph::ShapeLinker<num_t, 4, true , false, true , false>;
	template<typename num_t> using poly2dl_t  = graph::ShapeLinker<num_t, 2, false, true , false, false>;
	template<typename num_t> using poly3dl_t  = graph::ShapeLinker<num_t, 3, false, true , false, false>;
	template<typename num_t> using poly4dl_t  = graph::ShapeLinker<num_t, 4, false, true , false, false>;
	template<typename num_t> using cpoly2dl_t = graph::ShapeLinker<num_t, 2, true , true , false, false>;
	template<typename num_t> using cpoly3dl_t = graph::ShapeLinker<num_t, 3, true , true , false, false>;
	template<typename num_t> using cpoly4dl_t = graph::ShapeLinker<num_t, 4, true , true , false, false>;
	template<typename num_t> using tpoly2dl_t = graph::ShapeLinker<num_t, 2, false, true , true , false>;
	template<typename num_t> using tpoly3dl_t = graph::ShapeLinker<num_t, 3, false, true , true , false>;
	template<typename num_t> using tpoly4dl_t = graph::ShapeLinker<num_t, 4, false, true , true , false>;
	template<typename num_t> using shp2dl_t   = graph::ShapeLinker<num_t, 2, false, false, false, true >;
	template<typename num_t> using shp3dl_t   = graph::ShapeLinker<num_t, 3, false, false, false, true >;
	template<typename num_t> using shp4dl_t   = graph::ShapeLinker<num_t, 4, false, false, false, true >;
	template<typename num_t> using cshp2dl_t  = graph::ShapeLinker<num_t, 2, true , false, false, true >;
	template<typename num_t> using cshp3dl_t  = graph::ShapeLinker<num_t, 3, true , false, false, true >;
	template<typename num_t> using cshp4dl_t  = graph::ShapeLinker<num_t, 4, true , false, false, true >;
	template<typename num_t> using tshp2dl_t  = graph::ShapeLinker<num_t, 2, false, false, true , true >;
	template<typename num_t> using tshp3dl_t  = graph::ShapeLinker<num_t, 3, false, false, true , true >;
	template<typename num_t> using tshp4dl_t  = graph::ShapeLinker<num_t, 4, false, false, true , true >;
	template<typename num_t> using cplx2dl_t  = graph::ShapeLinker<num_t, 2, false, true , false, true >;
	template<typename num_t> using cplx3dl_t  = graph::ShapeLinker<num_t, 3, false, true , false, true >;
	template<typename num_t> using cplx4dl_t  = graph::ShapeLinker<num_t, 4, false, true , false, true >;
	template<typename num_t> using tcplx2dl_t = graph::ShapeLinker<num_t, 2, false, true , true , true >;
	template<typename num_t> using tcplx3dl_t = graph::ShapeLinker<num_t, 3, false, true , true , true >;
	template<typename num_t> using tcplx4dl_t = graph::ShapeLinker<num_t, 4, false, true , true , true >;
	template<typename num_t> using wllp2dl_t  = graph::ShapeLinker<num_t, 2, true , false, true , true >;
	template<typename num_t> using wllp3dl_t  = graph::ShapeLinker<num_t, 3, true , false, true , true >;
	template<typename num_t> using wllp4dl_t  = graph::ShapeLinker<num_t, 4, true , false, true , true >;
	template<typename num_t> using model2dl_t = graph::ShapeLinker<num_t, 2, true , true , false, true >;
	template<typename num_t> using model3dl_t = graph::ShapeLinker<num_t, 3, true , true , false, true >;
	template<typename num_t> using model4dl_t = graph::ShapeLinker<num_t, 4, true , true , false, true >;
	template<typename num_t> using fface2dl_t = graph::ShapeLinker<num_t, 2, true , true , true , false>;
	template<typename num_t> using fface3dl_t = graph::ShapeLinker<num_t, 3, true , true , true , false>;
	template<typename num_t> using fface4dl_t = graph::ShapeLinker<num_t, 4, true , true , true , false>;
	template<typename num_t> using mesh2dl_t  = graph::ShapeLinker<num_t, 2, true , true , true , true >;
	template<typename num_t> using mesh3dl_t  = graph::ShapeLinker<num_t, 3, true , true , true , true >;
	template<typename num_t> using mesh4dl_t  = graph::ShapeLinker<num_t, 4, true , true , true , true >;
	using f2dfl_t     = graph::ShapeLinker<float, 2, false, false, false, false>;
	using f3dfl_t     = graph::ShapeLinker<float, 3, false, false, false, false>;
	using f4dfl_t     = graph::ShapeLinker<float, 4, false, false, false, false>;
	using txr2dfl_t   = graph::ShapeLinker<float, 2, false, false, true , false>;
	using txr3dfl_t   = graph::ShapeLinker<float, 3, false, false, true , false>;
	using txr4dfl_t   = graph::ShapeLinker<float, 4, false, false, true , false>;
	using clrd2dfl_t  = graph::ShapeLinker<float, 2, true , false, false, false>;
	using clrd3dfl_t  = graph::ShapeLinker<float, 3, true , false, false, false>;
	using clrd4dfl_t  = graph::ShapeLinker<float, 4, true , false, false, false>;
	using ctxr2dfl_t  = graph::ShapeLinker<float, 2, true , false, true , false>;
	using ctxr3dfl_t  = graph::ShapeLinker<float, 3, true , false, true , false>;
	using ctxr4dfl_t  = graph::ShapeLinker<float, 4, true , false, true , false>;
	using poly2dfl_t  = graph::ShapeLinker<float, 2, false, true , false, false>;
	using poly3dfl_t  = graph::ShapeLinker<float, 3, false, true , false, false>;
	using poly4dfl_t  = graph::ShapeLinker<float, 4, false, true , false, false>;
	using cpoly2dfl_t = graph::ShapeLinker<float, 2, true , true , false, false>;
	using cpoly3dfl_t = graph::ShapeLinker<float, 3, true , true , false, false>;
	using cpoly4dfl_t = graph::ShapeLinker<float, 4, true , true , false, false>;
	using tpoly2dfl_t = graph::ShapeLinker<float, 2, false, true , true , false>;
	using tpoly3dfl_t = graph::ShapeLinker<float, 3, false, true , true , false>;
	using tpoly4dfl_t = graph::ShapeLinker<float, 4, false, true , true , false>;
	using shp2dfl_t   = graph::ShapeLinker<float, 2, false, false, false, true >;
	using shp3dfl_t   = graph::ShapeLinker<float, 3, false, false, false, true >;
	using shp4dfl_t   = graph::ShapeLinker<float, 4, false, false, false, true >;
	using cshp2dfl_t  = graph::ShapeLinker<float, 2, true , false, false, true >;
	using cshp3dfl_t  = graph::ShapeLinker<float, 3, true , false, false, true >;
	using cshp4dfl_t  = graph::ShapeLinker<float, 4, true , false, false, true >;
	using tshp2dfl_t  = graph::ShapeLinker<float, 2, false, false, true , true >;
	using tshp3dfl_t  = graph::ShapeLinker<float, 3, false, false, true , true >;
	using tshp4dfl_t  = graph::ShapeLinker<float, 4, false, false, true , true >;
	using cplx2dfl_t  = graph::ShapeLinker<float, 2, false, true , false, true >;
	using cplx3dfl_t  = graph::ShapeLinker<float, 3, false, true , false, true >;
	using cplx4dfl_t  = graph::ShapeLinker<float, 4, false, true , false, true >;
	using tcplx2dfl_t = graph::ShapeLinker<float, 2, false, true , true , true >;
	using tcplx3dfl_t = graph::ShapeLinker<float, 3, false, true , true , true >;
	using tcplx4dfl_t = graph::ShapeLinker<float, 4, false, true , true , true >;
	using wllp2dfl_t  = graph::ShapeLinker<float, 2, true , false, true , true >;
	using wllp3dfl_t  = graph::ShapeLinker<float, 3, true , false, true , true >;
	using wllp4dfl_t  = graph::ShapeLinker<float, 4, true , false, true , true >;
	using model2dfl_t = graph::ShapeLinker<float, 2, true , true , false, true >;
	using model3dfl_t = graph::ShapeLinker<float, 3, true , true , false, true >;
	using model4dfl_t = graph::ShapeLinker<float, 4, true , true , false, true >;
	using fface2dfl_t = graph::ShapeLinker<float, 2, true , true , true , false>;
	using fface3dfl_t = graph::ShapeLinker<float, 3, true , true , true , false>;
	using fface4dfl_t = graph::ShapeLinker<float, 4, true , true , true , false>;
	using mesh2dfl_t  = graph::ShapeLinker<float, 2, true , true , true , true >;
	using mesh3dfl_t  = graph::ShapeLinker<float, 3, true , true , true , true >;
	using mesh4dfl_t  = graph::ShapeLinker<float, 4, true , true , true , true >;



	constexpr c4f_t White     = c4f_t{1   ,1   ,1   ,1};
	constexpr c4f_t Red       = c4f_t{1   ,0   ,0   ,1};
	constexpr c4f_t Green     = c4f_t{0   ,1   ,0   ,1};
	constexpr c4f_t Blue      = c4f_t{0   ,0   ,1   ,1};
	constexpr c4f_t Yallow    = c4f_t{1   ,1   ,0   ,1};
	constexpr c4f_t Pinke     = c4f_t{1   ,0   ,1   ,1};
	constexpr c4f_t Orange    = c4f_t{1   ,0.5f,0   ,1};
	constexpr c4f_t Gray      = c4f_t{0.5f,0.5f,0.5f,1};
	constexpr c4f_t LightGray = c4f_t{0.8f,0.8f,0.8f,1};
	constexpr c4f_t Cyan      = c4f_t{0   ,1   ,1   ,1};
	constexpr c4f_t LightBlue = c4f_t{0   ,0.5f,1   ,1};
	constexpr c4f_t Black     = c4f_t{0   ,0   ,0   ,1};

	#include "inputs/keys.h++"
}
