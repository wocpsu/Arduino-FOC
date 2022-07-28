#include "foc_utils.h"

// int array instead of float array
// 4x200 points per 360 deg
// 2x storage save (int 2Byte float 4 Byte )
// sin*10000
const int sine_array[200] = {0,79,158,237,316,395,473,552,631,710,789,867,946,1024,1103,1181,1260,1338,1416,1494,1572,1650,1728,1806,1883,1961,2038,2115,2192,2269,2346,2423,2499,2575,2652,2728,2804,2879,2955,3030,3105,3180,3255,3329,3404,3478,3552,3625,3699,3772,3845,3918,3990,4063,4135,4206,4278,4349,4420,4491,4561,4631,4701,4770,4840,4909,4977,5046,5113,5181,5249,5316,5382,5449,5515,5580,5646,5711,5775,5839,5903,5967,6030,6093,6155,6217,6279,6340,6401,6461,6521,6581,6640,6699,6758,6815,6873,6930,6987,7043,7099,7154,7209,7264,7318,7371,7424,7477,7529,7581,7632,7683,7733,7783,7832,7881,7930,7977,8025,8072,8118,8164,8209,8254,8298,8342,8385,8428,8470,8512,8553,8594,8634,8673,8712,8751,8789,8826,8863,8899,8935,8970,9005,9039,9072,9105,9138,9169,9201,9231,9261,9291,9320,9348,9376,9403,9429,9455,9481,9506,9530,9554,9577,9599,9621,9642,9663,9683,9702,9721,9739,9757,9774,9790,9806,9821,9836,9850,9863,9876,9888,9899,9910,9920,9930,9939,9947,9955,9962,9969,9975,9980,9985,9989,9992,9995,9997,9999,10000,10000};
const uint16_t sin_lut[7] = {12540, 6393, 3212, 1608, 804, 402, 201};
const uint16_t cos_lut[7] = {30274, 32138, 32610, 32729, 32758, 32766, 32767};
const uint16_t tan_lut[6] = {32768, 13573, 6518, 3227, 1610, 804};

// function approximating the sine calculation by using fixed size array
// ~40us (float array)
// ~50us (int array)
// precision +-0.005
// it has to receive an angle in between 0 and 2PI
float _sin(float a){
  if(a < _PI_2){
    //return sine_array[(int)(199.0f*( a / (_PI/2.0)))];
    //return sine_array[(int)(126.6873f* a)];           // float array optimized
    return 0.0001f*sine_array[_round(126.6873f* a)];      // int array optimized
  }else if(a < _PI){
    // return sine_array[(int)(199.0f*(1.0f - (a-_PI/2.0) / (_PI/2.0)))];
    //return sine_array[398 - (int)(126.6873f*a)];          // float array optimized
    return 0.0001f*sine_array[398 - _round(126.6873f*a)];     // int array optimized
  }else if(a < _3PI_2){
    // return -sine_array[(int)(199.0f*((a - _PI) / (_PI/2.0)))];
    //return -sine_array[-398 + (int)(126.6873f*a)];           // float array optimized
    return -0.0001f*sine_array[-398 + _round(126.6873f*a)];      // int array optimized
  } else {
    // return -sine_array[(int)(199.0f*(1.0f - (a - 3*_PI/2) / (_PI/2.0)))];
    //return -sine_array[796 - (int)(126.6873f*a)];           // float array optimized
    return -0.0001f*sine_array[796 - _round(126.6873f*a)];      // int array optimized
  }
}

// function approximating cosine calculation by using fixed size array
// ~55us (float array)
// ~56us (int array)
// precision +-0.005
// it has to receive an angle in between 0 and 2PI
float _cos(float a){
  float a_sin = a + _PI_2;
  a_sin = a_sin > _2PI ? a_sin - _2PI : a_sin;
  return _sin(a_sin);
}


// normalizing radian angle to [0,2PI]
float _normalizeAngle(float angle){
  float a = fmod(angle, _2PI);
  return a >= 0 ? a : (a + _2PI);
}

// Electrical angle calculation
float _electricalAngle(float shaft_angle, int pole_pairs) {
  return (shaft_angle * pole_pairs);
}

// square root approximation function using
// https://reprap.org/forum/read.php?147,219210
// https://en.wikipedia.org/wiki/Fast_inverse_square_root
float _sqrtApprox(float number) {//low in fat
  long i;
  float y;
  // float x;
  // const float f = 1.5F; // better precision

  // x = number * 0.5F;
  y = number;
  i = * ( long * ) &y;
  i = 0x5f375a86 - ( i >> 1 );
  y = * ( float * ) &i;
  // y = y * ( f - ( x * y * y ) ); // better precision
  return number * y;
}
///Atan2 works on ESP32
void cordic10::atan2sqrt(int _y, int _x) {
	x0=_x; y0=_y; angle=0; sign=0;
	if(y0<0) {x0=-x0; y0=-y0; angle=512;}
	if(x0<0) {tmp=x0; x0=y0; y0=-tmp; angle+=256;}
	if(y0>x0) {sign=1; angle+=256; tmp=x0; x0=y0; y0=tmp;}
	currentAngle=64; phi=0;	k=0;
	while(k<=6){
		x1=(int)cos_lut[k]*(int)x0;
		x1+=(int)sin_lut[k]*(long)y0;
		y1=(int)cos_lut[k]*(int)y0;
		y1-=(int)sin_lut[k]*(int)x0;
		if(y1>=0) {x0=x1>>15; y0=y1>>15; phi+=currentAngle;}
		currentAngle=currentAngle>>1;
		k++;
    	}
	radius=x0;
	angle+=((sign)?-phi:+phi); //if(sign) {angle-=phi;} else {angle+=phi;}
	// angle 2pi = 1024
	angle=angle<<6; // stretch to the size of int. angle resolution is: 64
	// angle 2pi now is 65536
  }
  
  void cordic8::atan2sqrt(int _x, int _y) { 	//max input values for x and y must be confirmed.
	x0=_x; y0=_y; angle=0; sign=false;
	if(y0<0) {x0=-x0; y0=-y0; angle=128;} //rotate 128 cw
	if(x0<0) {tmp=x0; x0=y0; y0=-tmp; angle+=64;} //rotate 64 cw
	if(y0>x0) {sign=true; angle+=64; tmp=x0; x0=y0; y0=tmp;}
	currentAngle=16; phi=0; k=0;
	while (k<=4){
		x1=(long)cos_lut[k]*(long)x0;
		x1+=(long)sin_lut[k]*(long)y0;
		y1=-(long)sin_lut[k]*(long)x0;
		y1+=(long)cos_lut[k]*(long)y0;
		if (y1>=0) {x0=x1>>15; y0=y1>>15; phi+=currentAngle;}
		currentAngle=currentAngle>>1;  //means: currAngle=currAngle/2;
		k++;
    	}
	radius=x0;
	angle+=((sign)?-phi:+phi); //faster than: if(sign) {angle-=phi;} else {angle+=phi;} ? To be confirmed...
	// angle 2pi = 256, size of char
}
