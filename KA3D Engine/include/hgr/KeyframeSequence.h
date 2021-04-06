#ifndef _HGR_KEYFRAMESEQUENCE_H
#define _HGR_KEYFRAMESEQUENCE_H


#include <gr/VertexFormat.h>
#include <lang/Array.h>
#include <lang/Object.h>
#include <math/float4.h>


namespace hgr
{


/**
 * Keyframe sequence of flexible format keyframe data.
 * @ingroup hgr
 */
class KeyframeSequence :
	public lang::Object
{
public:
	/**
	 * Allocates array of specified key count and format.
	 */
	KeyframeSequence( int keys, gr::VertexFormat::DataFormat format );

	/**
	 * Removes keys which don't affect the sequence.
	 */
	void	removeRedundantKeys();

	/**
	 * Sets key data.
	 */
	void	setKeyframe( int index, const math::float4& v );

	/**
	 * Sets key data of variable format.
	 * @param index First key to set.
	 * @param data Pointer to source data.
	 * @param pitch Byte distance between keys in the data.
	 * @param df Format of source data.
	 * @param scale Scale for source data.
	 * @param bias Offset to add to source data after scaling.
	 * @param count Number of keys to set.
	 */
	void	setKeyframe( int index,
				const void* data, int pitch, gr::VertexFormat::DataFormat df, 
				float scale, const math::float4& bias, int count );

	/**
	 * Sets data scale. Final float4 value is source*scale+bias.
	 * Default scale is 1.
	 */
	void	setScale( float s );

	/**
	 * Sets data bias. Final float4 value is source*scale+bias.
	 * Default bias is (0,0,0,0).
	 */
	void	setBias( const math::float4 bias );

	/**
	 * Sets number of keys in the array.
	 */
	void	setKeys( int keys );

	/**
	 * Returns access to key data.
	 */
	void*	data()				{return &m_data[0];}

	/**
	 * Gets key data. Applies scaling and bias before to returned data.
	 * @param index First key to set.
	 * @param data Pointer to source data.
	 * @param pitch Byte distance between keys in the data.
	 * @param df Format of source data.
	 * @param scale Scale for destination data (applied before data usage).
	 * @param bias Offset to add to destination data after scaling.
	 * @param count Number of keys to get.
	 */
	math::float4	getKeyframe( int index ) const;

	/**
	 * Gets key data. Applies scaling and bias before to returned data.
	 * @param index First key to set.
	 * @param data Pointer to source data.
	 * @param pitch Byte distance between keys in the data.
	 * @param df Format of source data.
	 * @param scale Scale for destination data (applied before data usage).
	 * @param bias Offset to add to destination data after scaling.
	 * @param count Number of keys to get.
	 */
	void	getKeyframe( int index,
				void* data, int pitch, gr::VertexFormat::DataFormat df, int count ) const;

	/**
	 * Returns min/max bounds of data.
	 */
	void	getBound( math::float4* minv, math::float4* maxv ) const;

	/**
	 * Returns number of keys in the array.
	 */
	int					keys() const;

	/**
	 * Returns data format of this key array.
	 */
	gr::VertexFormat::DataFormat	format() const;

	/**
	 * Returns data scale.
	 */
	float				scale() const;
	
	/**
	 * Returns data bias.
	 */
	const math::float4&	bias() const;

	/**
	 * Returns access to key data.
	 */
	const void*			data() const		{return &m_data[0];}

	/**
	 * Returns the sequence packed to new format.
	 * @param format Data format of the new KeyframeSequence.
	 * @param maxvalue Maximum value (+-) of a component in the new format.
	 */
	P(KeyframeSequence)	getPacked( gr::VertexFormat::DataFormat format, float maxvalue ) const;

private:
	lang::Array<uint8_t>			m_data;
	int								m_keys;
	gr::VertexFormat::DataFormat	m_format;
	float							m_scale;
	math::float4					m_bias;

	int					pitch() const		{return gr::VertexFormat::getDataSize(m_format);}
};


} // hgr


#endif // _HGR_KEYFRAMESEQUENCE_H
