#ifndef _HGR_LIGHTSORTER_H
#define _HGR_LIGHTSORTER_H


#include <hgr/Light.h>
#include <lang/Array.h>


namespace hgr
{


class Light;


/**
 * Helper class for sorting lights by distance.
 * Used to select the most important lights in object rendering.
 *
 * @ingroup hgr
 */
class LightSorter
{
public:
	/**
	 * Constructs empty light sorter.
	 */
	LightSorter();

	/**
	 * Returns array of lights sorted by distance to the world space position. 
	 * Closest lights become first.
	 */
	lang::Array<Light*>&	getLightsByDistance( const math::float3& worldpos, int maxlights=8 );

	/**
	 * Adds a new collected light.
	 */
	void		addLight( Light* obj );

	/**
	 * Removes all collected lights.
	 */
	void		removeLights();

	/**
	 * Collects lights from scene to sm_lightdata and sm_lightsorter.
	 * Used to get quickly the lights which are near the object.
	 * Ignores disabled lights.
	 */
	void		collectLights( Node* root );

	/**
	 * Returns ith light.
	 */
	hgr::Light*	getLight( int i )					{return m_lightData[i].obj;}

	/**
	 * Returns number of lights added.
	 */
	int			lights() const						{return m_lightData.size();}

private:
	class LightData
	{
	public:
		float v;
		P(Light) obj;
		math::float3 wpos;
	};

	class LightSortValue
	{
	public:
		LightData* data;

		bool operator<( const LightSortValue& other ) const		{return data->v < other.data->v;}
	};

	lang::Array<Light*>			m_lights;
	lang::Array<LightData>		m_lightData;
	lang::Array<LightSortValue>	m_lightSorter;
};


} // hgr


#endif // _HGR_LIGHTSORTER_H

// Copyright (C) 2005 Jani Kajala. All rights reserved. Consult your license regarding permissions and restrictions.
