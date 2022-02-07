#pragma once

namespace Engine
{
	typedef unsigned char				_ubyte;
	typedef signed char					_byte;

	typedef unsigned short				_ushort;
	typedef signed short				_short;

	typedef unsigned int				_uint;
	typedef signed int					_int;

	typedef unsigned long				_ulong;
	typedef signed long					_long;

	typedef float						_float;
	typedef double						_double;

	typedef bool						_bool;

	typedef wchar_t						_tchar;

	/*typedef struct tagMyVector3 : public D3DXVECTOR3
	{
		float Get_Length() {
			
		}
		float Get_Distance(tagMyVector3 vTarget) {
			
		}
	}_float3;*/

	typedef D3DXVECTOR2					_float2;
	typedef D3DXVECTOR3					_float3;
	typedef D3DXVECTOR4					_float4;

	typedef D3DXMATRIX					_float4x4;
}