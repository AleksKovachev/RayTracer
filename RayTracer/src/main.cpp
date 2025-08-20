// TODO: Finish the interactive rendering.
// TODO: Finish the OBJ file parser.
// TODO: Implement reading and applying Normal maps.
// TODO: Implement Global Illumination.
// TODO: Implement BVH Tree and SAH.
// TODO: Implement bitmap texture mapping without stretching. 2 types - Fit, which
//       puts black bars around the texture to fit the geometry, and Fill, which
//       places the texture making it go "out of bounds" but fill the geometry.
// TODO: Make each bucket in bucket rendering save the rendered image to see the
//       progress. Make it as a separate function, as it would slow donw the process.
// TODO: Add blurring option.
// TODO: Implement Ambient Occlusion (RTAO) and SSAO (Screen Space Ambient Occlusion).


/*
 * The whole Render Engine is built for ease of use, which costs a lot of memory.
 * The program should only work with the Mesh class. The IntersectionData class
 * should store object index, and triangle index to work with. In this case,
 * each triangle has to be constructed for every iteration inside the
 * GetTriangleIntersection function, for every call. This results in a lot of
 * repetitive work, but saves resources.
 */
