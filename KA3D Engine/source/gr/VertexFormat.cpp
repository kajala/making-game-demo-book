#include <gr/VertexFormat.h>
#include <math/float4.h>
#include <stdio.h>
#include <string.h>
#include <config.h>


using namespace lang;
using namespace math;


namespace gr
{


static const char* const DATATYPE_NAMES[] =
{
	/** Vertex has model space position data. */
	"DT_POSITION",
	/** Vertex has screen space position data. */
	"DT_POSITIONT",
	/** Vertex has bone weights used in skinning. */
	"DT_BONEWEIGHTS",
	/** Vertex has bone indices used in skinning. */
	"DT_BONEINDICES",
	/** Vertex has normal pointing away from surface. */
	"DT_NORMAL",
	/** Vertex has diffuse color data. */
	"DT_DIFFUSE",
	/** Vertex has specular color data. */
	"DT_SPECULAR",
	/** Vertex has texture layer 0. */
	"DT_TEX0",
	/** Vertex has texture layer 1. */
	"DT_TEX1",
	/** Vertex has texture layer 2. */
	"DT_TEX2",
	/** Vertex has texture layer 3. */
	"DT_TEX3",
	/** Vertex has tangent data. */
	"DT_TANGENT",
};

const char* const DATAFORMAT_NAMES[] =
{
	/** Null vertex data format. */
	"NONE",
	/** Single 32-bit float. */
	"S_32",
	/** Single 16-bit unsigned integer. */
	"S_16",
	/** Single 8-bit unsigned integer. */
	"S_8",
	/** 32-bit float 2-vector. */
	"V2_32",
	/** 16-bit unsigned integer 2-vector. */
	"V2_16",
	/** 8-bit unsigned integer 2-vector. */
	"V2_8",
	/** 32-bit float 3-vector. */
	"V3_32",
	/** 16-bit unsigned integer 3-vector. */
	"V3_16",
	/** 8-bit unsigned integer 3-vector. */
	"V3_8",
	/** 32-bit float 4-vector. */
	"V4_32",
	/** 16-bit unsigned integer 4-vector. */
	"V4_16",
	/** 8-bit unsigned integer 4-vector. */
	"V4_8",
	/** 5-bit unsigned integer 4-vector. */
	"V4_5",
};


VertexFormat::VertexFormat()
{
	assert( sizeof(m_data)*2 >= DT_SIZE ); // too little space for data types

	for ( int i = 0 ; i < DT_SIZE ; ++i )
		setDataFormat( (DataType)i, DF_NONE );
}

VertexFormat& VertexFormat::addPosition( DataFormat df )
{
	assert( getDataFormat(DT_POSITION) == DF_NONE );
	assert( getDataFormat(DT_POSITIONT) == DF_NONE );
	
	setDataFormat( DT_POSITION, df );
	return *this;
}

VertexFormat& VertexFormat::addTransformedPosition( DataFormat df )
{
	assert( getDataFormat(DT_POSITION) == DF_NONE );
	assert( getDataFormat(DT_POSITIONT) == DF_NONE );
	assert( getDataFormat(DT_NORMAL) == DF_NONE ); // excludes vertex normal
	
	setDataFormat( DT_POSITIONT, df );
	return *this;
}

VertexFormat& VertexFormat::addNormal( DataFormat df )
{
	assert( getDataFormat(DT_NORMAL) == DF_NONE );
	assert( getDataFormat(DT_POSITIONT) == DF_NONE ); // excludes transformed vertices

	setDataFormat( DT_NORMAL, df );
	assert( getDataFormat(DT_NORMAL) == df );
	return *this;
}

VertexFormat& VertexFormat::addDiffuse( DataFormat df )
{
	assert( getDataFormat(DT_DIFFUSE) == DF_NONE );

	setDataFormat( DT_DIFFUSE, df );
	return *this;
}
	
VertexFormat& VertexFormat::addSpecular( DataFormat df )
{
	assert( getDataFormat(DT_SPECULAR) == DF_NONE );

	setDataFormat( DT_SPECULAR, df );
	return *this;
}

VertexFormat& VertexFormat::addTangent( DataFormat df )
{
	assert( getDataFormat(DT_TANGENT) == DF_NONE );

	setDataFormat( DT_TANGENT, df );
	return *this;
}

VertexFormat& VertexFormat::addWeights( DataFormat weightdf, DataFormat indexdf )
{
	assert( getDataFormat(DT_BONEWEIGHTS) == DF_NONE );
	assert( getDataFormat(DT_BONEINDICES) == DF_NONE );
	
	setDataFormat( DT_BONEWEIGHTS, weightdf );
	setDataFormat( DT_BONEINDICES, indexdf );
	return *this;
}

VertexFormat& VertexFormat::addTextureCoordinate( DataFormat df )
{
	int layer = textureCoordinates();
	assert( layer < MAX_TEXCOORDS && "Too many texture coordinate layers in vertex format" );
	if ( layer < MAX_TEXCOORDS )
		setDataFormat( (DataType)(DT_TEX0+layer), df );
	return *this;
}

int VertexFormat::textureCoordinates() const
{
	int layers = 0;
	if ( m_data[DT_TEX0>>1] & (0xF<<((DT_TEX0&1)<<2)) )
	{
		++layers;
		if ( m_data[DT_TEX1>>1] & (0xF<<((DT_TEX1&1)<<2)) )
		{
			++layers;
			if ( m_data[DT_TEX2>>1] & (0xF<<((DT_TEX2&1)<<2)) )
			{
				++layers;
				if ( m_data[DT_TEX3>>1] & (0xF<<((DT_TEX3&1)<<2)) )
					++layers;
			}
		}
	}

#ifdef _DEBUG
	int dbglayers = 0;
	for ( int i = DT_TEX0 ; i < DT_TEX0+MAX_TEXCOORDS ; ++i )
		if ( hasData(DataType(i)) )
			++dbglayers;
	assert( dbglayers == layers && "VertexFormat::textureCoordinates returns incorrect value" );
#endif

	return layers;
}

VertexFormat::DataFormat VertexFormat::getTextureCoordinateFormat( int layer ) const
{
	assert( layer < MAX_TEXCOORDS && "Not enough texture coordinate layers in vertex format" );
	if ( layer < MAX_TEXCOORDS )
		return getDataFormat( (DataType)(DT_TEX0+layer) );
	else
		return DF_NONE;
}

void VertexFormat::setDataFormat( DataType dt, DataFormat value )
{
	assert( (unsigned)dt < sizeof(m_data)*2 );
	assert( value >= 0 && value <= 0xF );

	int index = dt;
	int shift = (index&1) << 2;
	index >>= 1;
	uint8_t* data = reinterpret_cast<uint8_t*>( m_data );
	data[index] = (uint8_t)( ((int)data[index] & (0xF0>>shift)) + (value << shift) );
}

VertexFormat::DataFormat VertexFormat::getDataFormat( DataType dt ) const
{
	assert( (unsigned)dt < (unsigned)DT_SIZE );
	
	int index = dt;
	int shift = (index&1) << 2;
	index >>= 1;
	const uint8_t* data = reinterpret_cast<const uint8_t*>( m_data );
	return (DataFormat)(  ( (int)data[index] & (0xF<<shift) ) >> shift  );
}

String VertexFormat::toString() const
{
	char str[DT_SIZE*50];
	char buf[50];
	bool first = true;
	String::cpy( str, sizeof(str), "(" );
	for ( int i = 0 ; i < DT_SIZE ; ++i )
	{
		DataType dt = DataType(i);
		DataFormat df = getDataFormat(dt);
		if ( df != DF_NONE )
		{
			if ( !first )
				String::cat( str, sizeof(str), ", " );
			sprintf( buf, "%s=%s", toString(dt), toString(df) );
			String::cat( str, sizeof(str), buf );
			first = false;
		}
	}
	String::cat( str, sizeof(str), ")" );
	return str;
}

int VertexFormat::getDataSize( DataFormat df )
{
	switch ( df )
	{
		case DF_S_32:	return 4;
		case DF_S_16:	return 2;
		case DF_S_8:	return 1;
		case DF_V2_32:	return 8;
		case DF_V2_16:	return 4;
		case DF_V2_8:	return 2;
		case DF_V3_32:	return 12;
		case DF_V3_16:	return 6;
		case DF_V3_8:	return 3;
		case DF_V4_32:	return 16;
		case DF_V4_16:	return 8;
		case DF_V4_8:	return 4;
		case DF_V4_5:	return 2;
		case DF_NONE:	return 0;
		case DF_SIZE:	return 0;
	}
	return 0;
}

int VertexFormat::getDataDim( DataFormat df )
{
	switch ( df )
	{
		case DF_S_32:	return 1;
		case DF_S_16:	return 1;
		case DF_S_8:	return 1;
		case DF_V2_32:	return 2;
		case DF_V2_16:	return 2;
		case DF_V2_8:	return 2;
		case DF_V3_32:	return 3;
		case DF_V3_16:	return 3;
		case DF_V3_8:	return 3;
		case DF_V4_32:	return 4;
		case DF_V4_16:	return 4;
		case DF_V4_8:	return 4;
		case DF_V4_5:	return 4;
		case DF_NONE:	return 0;
		case DF_SIZE:	return 0;
	}
	return 0;
}

int VertexFormat::getDataSize( DataFormat df, int n )
{
	int bytes = 0;
	switch ( df )
	{
		case DF_S_32:	bytes = n<<2; break;
		case DF_S_16:	bytes = n+n; break;
		case DF_S_8:	bytes = n; break;
		case DF_V2_32:	bytes = n<<3; break;
		case DF_V2_16:	bytes = n<<2; break;
		case DF_V2_8:	bytes = n+n; break;
		case DF_V3_32:	bytes = (n<<3) + (n<<2); break;
		case DF_V3_16:	bytes = (n<<2) + (n+n); break;
		case DF_V3_8:	bytes = n+n+n; break;
		case DF_V4_32:	bytes = n<<4; break;
		case DF_V4_16:	bytes = n<<3; break;
		case DF_V4_8:	bytes = n<<2; break;
		case DF_V4_5:	bytes = n+n; break;
		case DF_NONE:	return 0;
		case DF_SIZE:	return 0;
	}
	
	// align to 32-bit boundary
	return (bytes+3) & ~3;
}

const char*	VertexFormat::toString( DataFormat df )
{
	assert( sizeof(DATAFORMAT_NAMES)/sizeof(DATAFORMAT_NAMES[0]) == DF_SIZE );
	assert( df < DF_SIZE );
	return DATAFORMAT_NAMES[df];
}

const char*	VertexFormat::toString( DataType dt )
{
	assert( sizeof(DATATYPE_NAMES)/sizeof(DATATYPE_NAMES[0]) == DT_SIZE );
	assert( dt < DT_SIZE );
	return DATATYPE_NAMES[dt];
}

int VertexFormat::vertexSize() const
{
	int size = 0;
	for ( int i = 0 ; i < DT_SIZE ; ++i )
	{
		DataType dt = (DataType)i;
		if ( hasData(dt) )
			size += getDataSize( getDataFormat(dt) );
	}
	return size;
}

bool VertexFormat::operator==( const VertexFormat& other ) const
{
	for ( int i = 0 ; i < DT_SIZE ; ++i )
		if ( getDataFormat( (DataType)i ) != other.getDataFormat( (DataType)i ) )
			return false;
	return true;
}

void VertexFormat::getData( DataFormat df, const void* data, float4* out )
{
	switch ( df )
	{
	case DF_S_32:
		out->x = reinterpret_cast<const float*>(data)[0];
		out->y = 0.f;
		out->z = 0.f;
		out->w = 0.f;
		break;

	case DF_S_16:	
		out->x = reinterpret_cast<const uint16_t*>(data)[0];
		out->y = 0.f;
		out->z = 0.f;
		out->w = 0.f;
		break;

	case DF_S_8:	
		out->x = reinterpret_cast<const uint8_t*>(data)[0];
		out->y = 0.f;
		out->z = 0.f;
		out->w = 0.f;
		break;

	case DF_V2_32:
		out->x = reinterpret_cast<const float*>(data)[0];
		out->y = reinterpret_cast<const float*>(data)[1];
		out->z = 0.f;
		out->w = 0.f;
		break;

	case DF_V2_16:	
		out->x = reinterpret_cast<const uint16_t*>(data)[0];
		out->y = reinterpret_cast<const uint16_t*>(data)[1];
		out->z = 0.f;
		out->w = 0.f;
		break;

	case DF_V2_8:	
		out->x = reinterpret_cast<const uint8_t*>(data)[0];
		out->y = reinterpret_cast<const uint8_t*>(data)[1];
		out->z = 0.f;
		out->w = 0.f;
		break;

	case DF_V3_32:
		out->x = reinterpret_cast<const float*>(data)[0];
		out->y = reinterpret_cast<const float*>(data)[1];
		out->z = reinterpret_cast<const float*>(data)[2];
		out->w = 0.f;
		break;

	case DF_V3_16:	
		out->x = reinterpret_cast<const uint16_t*>(data)[0];
		out->y = reinterpret_cast<const uint16_t*>(data)[1];
		out->z = reinterpret_cast<const uint16_t*>(data)[2];
		out->w = 0.f;
		break;

	case DF_V3_8:	
		out->x = reinterpret_cast<const uint8_t*>(data)[0];
		out->y = reinterpret_cast<const uint8_t*>(data)[1];
		out->z = reinterpret_cast<const uint8_t*>(data)[2];
		out->w = 0.f;
		break;

	case DF_V4_32:
		out->x = reinterpret_cast<const float*>(data)[0];
		out->y = reinterpret_cast<const float*>(data)[1];
		out->z = reinterpret_cast<const float*>(data)[2];
		out->w = reinterpret_cast<const float*>(data)[3];
		break;

	case DF_V4_16:	
		out->x = reinterpret_cast<const uint16_t*>(data)[0];
		out->y = reinterpret_cast<const uint16_t*>(data)[1];
		out->z = reinterpret_cast<const uint16_t*>(data)[2];
		out->w = reinterpret_cast<const uint16_t*>(data)[3];
		break;

	case DF_V4_8:	
		out->x = reinterpret_cast<const uint8_t*>(data)[0];
		out->y = reinterpret_cast<const uint8_t*>(data)[1];
		out->z = reinterpret_cast<const uint8_t*>(data)[2];
		out->w = reinterpret_cast<const uint8_t*>(data)[3];
		break;

	case DF_V4_5:{
		const uint16_t* s = reinterpret_cast<const uint16_t*>(data);
		out->x = float(s[0] & 0x1F);
		out->y = float((s[0]>>5) & 0x1F);
		out->z = float((s[0]>>10) & 0x1F);
		out->w = float((s[0]>>15) & 1);
		break;}

	case DF_NONE:
		break;

	case DF_SIZE:
		break;
	}
}

void VertexFormat::setData( DataFormat df, void* data, const float4& v )
{
	switch ( df )
	{
	case VertexFormat::DF_V4_32:{
		float* d = reinterpret_cast<float*>(data);
		d[0] = v.x;
		d[1] = v.y;
		d[2] = v.z;
		d[3] = v.w;
		break;}

	case VertexFormat::DF_V3_32:{
		float* d = reinterpret_cast<float*>(data);
		d[0] = v.x;
		d[1] = v.y;
		d[2] = v.z;
		break;}

	case VertexFormat::DF_V2_32:{
		float* d = reinterpret_cast<float*>(data);
		d[0] = v.x;
		d[1] = v.y;
		break;}

	case VertexFormat::DF_S_32:{
		float* d = reinterpret_cast<float*>(data);
		d[0] = v.x;
		break;}
		
	case VertexFormat::DF_V4_16:{
		uint16_t* d = reinterpret_cast<uint16_t*>(data);
		d[0] = (uint16_t)v.x;
		d[1] = (uint16_t)v.y;
		d[2] = (uint16_t)v.z;
		d[3] = (uint16_t)v.w;
		break;}

	case VertexFormat::DF_V3_16:{
		uint16_t* d = reinterpret_cast<uint16_t*>(data);
		d[0] = (uint16_t)v.x;
		d[1] = (uint16_t)v.y;
		d[2] = (uint16_t)v.z;
		break;}

	case VertexFormat::DF_V2_16:{
		uint16_t* d = reinterpret_cast<uint16_t*>(data);
		d[0] = (uint16_t)v.x;
		d[1] = (uint16_t)v.y;
		break;}

	case VertexFormat::DF_S_16:{
		uint16_t* d = reinterpret_cast<uint16_t*>(data);
		d[0] = (uint16_t)v.x;
		break;}

	case VertexFormat::DF_V4_8:{
		uint8_t* d = reinterpret_cast<uint8_t*>(data);
		d[0] = (uint8_t)v.x;
		d[1] = (uint8_t)v.y;
		d[2] = (uint8_t)v.z;
		d[3] = (uint8_t)v.w;
		break;}

	case VertexFormat::DF_V3_8:{
		uint8_t* d = reinterpret_cast<uint8_t*>(data);
		d[0] = (uint8_t)v.x;
		d[1] = (uint8_t)v.y;
		d[2] = (uint8_t)v.z;
		break;}

	case VertexFormat::DF_V2_8:{
		uint8_t* d = reinterpret_cast<uint8_t*>(data);
		d[0] = (uint8_t)v.x;
		d[1] = (uint8_t)v.y;
		break;}

	case VertexFormat::DF_S_8:{
		uint8_t* d = reinterpret_cast<uint8_t*>(data);
		d[0] = (uint8_t)v.x;
		break;}

	case VertexFormat::DF_V4_5:{
		uint16_t* d = reinterpret_cast<uint16_t*>(data);
		d[0] = (uint16_t)( ((int32_t)v.x & 0x1F)
			+ (((int32_t)v.y & 0x1F) << 5)
			+ (((int32_t)v.z & 0x1F) << 10)
			+ (((int32_t)v.w != 0) << 15) );
		break;}
		
	default:
		assert( false ); // unsupported conversion
	}
}

void VertexFormat::copyData(
	void* ddata, int dpitch, DataFormat df,
	const void* sdata, int spitch, DataFormat sf,
	const float4& sscale, const float4& sbias, int count )
{
	for ( int i = 0 ; i < count ; ++i )
	{
		float4 v(0,0,0,0);

		// get data float4
		VertexFormat::getData( sf, sdata, &v );
		sdata = reinterpret_cast<const uint8_t*>(sdata) + spitch;

		// new scale & bias
		//v = v*(scale0*scale1inv) + (bias0-bias1)*scale1inv;
		v *= sscale;
		v += sbias;

		// set data float4
		VertexFormat::setData( df, ddata, v );
		ddata = reinterpret_cast<uint8_t*>(ddata) + dpitch;
	}
}

VertexFormat::DataFormat VertexFormat::toDataFormat( const char* str )
{
	for ( int i = 0 ; i < DF_SIZE ; ++i )
		if ( !strcmp(toString(DataFormat(i)),str) )
			return DataFormat(i);
	return DF_SIZE;
}

VertexFormat::DataType VertexFormat::toDataType( const char* str )
{
	for ( int i = 0 ; i < DT_SIZE ; ++i )
		if ( !strcmp(toString(DataType(i)),str) )
			return DataType(i);
	return DT_SIZE;
}


} // gr

// Copyright (C) 2005 Jani Kajala. All rights reserved. Consult your license regarding permissions and restrictions.
