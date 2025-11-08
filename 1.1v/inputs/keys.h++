namespace key {
	constexpr int Released    =  0;
	constexpr int Pressed     =  1;
	constexpr int Spamed      =  2;

	constexpr int space       = 32;
	constexpr int apostrophe  = 39;
	constexpr int comma       = 44;
	constexpr int minus       = 45;
	constexpr int period      = 46;
	constexpr int slash       = 47;

	constexpr int _0          = 48;
	constexpr int _1          = 49;
	constexpr int _2          = 50;
	constexpr int _3          = 51;
	constexpr int _4          = 52;
	constexpr int _5          = 53;
	constexpr int _6          = 54;
	constexpr int _7          = 55;
	constexpr int _8          = 56;
	constexpr int _9          = 57;

	constexpr int semicolon   = 59;
	constexpr int equal       = 61;

	constexpr int a           = 65;
	constexpr int b           = 66;
	constexpr int c           = 67;
	constexpr int d           = 68;
	constexpr int e           = 69;
	constexpr int f           = 70;
	constexpr int g           = 71;
	constexpr int h           = 72;
	constexpr int i           = 73;
	constexpr int j           = 74;
	constexpr int k           = 75;
	constexpr int l           = 76;
	constexpr int m           = 77;
	constexpr int n           = 78;
	constexpr int o           = 79;
	constexpr int p           = 80;
	constexpr int q           = 81;
	constexpr int r           = 82;
	constexpr int s           = 83;
	constexpr int t           = 84;
	constexpr int u           = 85;
	constexpr int v           = 86;
	constexpr int w           = 87;
	constexpr int x           = 88;
	constexpr int y           = 89;
	constexpr int z           = 90;

	constexpr int l_bracket   = 91;
	constexpr int backslash   = 92;
	constexpr int r_bracket   = 93;
	constexpr int grave_accent= 96;
	constexpr int word_1      =161;
	constexpr int word_2      =162;

	constexpr int escape      =256;
	constexpr int enter       =257;
	constexpr int tab         =258;
	constexpr int backspace   =259;
	constexpr int insert      =260;
	constexpr int delete_     =261;

	constexpr int right       =262;
	constexpr int left        =263;
	constexpr int down        =264;
	constexpr int up          =265;

	constexpr int page_up     =266;
	constexpr int page_down   =267;
	constexpr int home        =268;
	constexpr int end         =269;

	constexpr int caps_lock   =280;
	constexpr int scroll_lock =281;
	constexpr int num_lock    =282;
	constexpr int print_screen=283;
	constexpr int pause       =284;

	constexpr int f1          =290;
	constexpr int f2          =291;
	constexpr int f3          =292;
	constexpr int f4          =293;
	constexpr int f5          =294;
	constexpr int f6          =295;
	constexpr int f7          =296;
	constexpr int f8          =297;
	constexpr int f9          =298;
	constexpr int f10         =299;
	constexpr int f11         =300;
	constexpr int f12         =301;
	constexpr int f13         =302;
	constexpr int f14         =303;
	constexpr int f15         =304;
	constexpr int f16         =305;
	constexpr int f17         =306;
	constexpr int f18         =307;
	constexpr int f19         =308;
	constexpr int f20         =309;
	constexpr int f21         =310;
	constexpr int f22         =311;
	constexpr int f23         =312;
	constexpr int f24         =313;
	constexpr int f25         =314;

	constexpr int kp_0        =320;
	constexpr int kp_1        =321;
	constexpr int kp_2        =322;
	constexpr int kp_3        =323;
	constexpr int kp_4        =324;
	constexpr int kp_5        =325;
	constexpr int kp_6        =326;
	constexpr int kp_7        =327;
	constexpr int kp_8        =328;
	constexpr int kp_9        =329;

	constexpr int kp_decimal  =330;
	constexpr int kp_dev      =331;
	constexpr int kp_mul      =332;
	constexpr int kp_sub      =333;
	constexpr int kp_add      =334;
	constexpr int kp_enter    =335;
	constexpr int kp_equal    =336;
	constexpr int l_shift     =340;
	constexpr int l_control   =341;
	constexpr int l_alt       =342;
	constexpr int l_super     =343;

	constexpr int r_shift     =344;
	constexpr int r_control   =345;
	constexpr int r_alt       =346;
	constexpr int r_super     =347;

	constexpr int menu        =348;
	constexpr int last        =menu;

	constexpr std::array<int, 100> default_keys_list ={
		 32, 39, 44, 45, 46, 47, 48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 59, 61, 65, 66,
		 67, 68, 69, 70, 71, 72, 73, 74, 75, 76, 77, 78, 79, 80, 81, 82, 83, 84, 85, 86,
		 87, 88, 89, 90, 91, 92, 93, 96,161,162,256,257,258,259,260,261,262,263,264,265,
		266,267,268,269,280,281,282,283,284,290,291,292,293,294,295,296,297,298,299,300,
		301,302,303,304,305,306,307,308,309,310,311,312,313,314,320,321,322,346,347,348
	};
}
