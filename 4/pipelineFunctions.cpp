/* This file contains the definitions of the pipeline transformation functions */

#include <pipelineFunctions.h>

#define X_INCR 1
#define Y_INCR 1

/* This function is a part of the depthBuffer function for a list of polygons.
 * This function runs on a polygon and returns a pair of DepthBuffer and 
 * RefreshBuffer for that polygon. The result is then incorporated in the 
 * function dealing with a list of polygons.
 */
pair<DepthBuffer, RefreshBuffer> depthBufferMethod (const Polygon& polygon)
{
	DepthBuffer depth;
	RefreshBuffer refresh;

	double x, y, z;

	// Compute 'z' TODO

	for ( x = polygon.xmin(); x < polygon.xmax(); x += X_INCR)
	{
		for ( y = polygon.ymin(); y < polygon.ymax(); y += Y_INCR)
		{
			// Since its only one polygon, just insert the values in refresh and depth buffer.
			depth[Pt2D(x,y)]   = z;
			refresh[Pt2D(x,y)] = polygon.getColor();
		}
	}

	return make_pair (depth, refresh);
}

map<Pt2D,Color> depthBufferMethod (const list<Polygon>& polygons)
{
	list<Polygon>::const_iterator itr;
	DepthBuffer depth;
	RefreshBuffer refresh;

	for (itr = polygons.begin(); itr != polygons.end(); itr++)
	{
		// Call the depth buffer function for each polygon.
		pair<DepthBuffer, RefreshBuffer> tmpPair = depthBuffer (*itr);
		DepthBuffer tmpDepth = tmpPair.first;
		RefreshBuffer tmpRefresh = tmpPair.second;

		// Update main depth-buffer and refresh buffer.
		DepthBuffer::iterator itrDepth;
		RefreshBuffer::iterator itrRefresh;
		for ( itrDepth = tmpDepth.begin(); itrDepth != tmpDepth.end(); itrDepth++)
		{
			DepthBuffer::iterator itrTmp;
			if ( (itrTmp = depth.find(itr->first)) != depth.end())
			{
				// If found in existing table
				if ( itr->second > itrTmp->second )
				{
					itrTmp->second = itrDepth->second;
					refresh[itrDepth->second] = tmpRefresh[itrDepth->first];
				}
			}
			else
			{
				// if it is not found in the existing table, add it.
				depth[itrDepth->first]   = itrDepth->second;
				refresh[itrDepth->first] = tmpRefresh[itrDepth->second];
			}
		}
	}

	return refresh;
}