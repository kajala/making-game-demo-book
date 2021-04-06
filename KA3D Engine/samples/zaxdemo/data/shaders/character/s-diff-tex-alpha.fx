//
// Simple gouraud diffuse shader with texture and 1 point light and transparency.
//
// Copyright (C) 2004 Jani Kajala. All rights reserved. Consult your license regarding permissions and restrictions.
//

#ifdef ENGINE_RUNTIME
#define ENABLE_SKINNING
#endif

#define ENABLE_TRANSPARENCY
#define ENABLE_SORT "BACKTOFRONT"
#include "../level/diff-tex.fx"
