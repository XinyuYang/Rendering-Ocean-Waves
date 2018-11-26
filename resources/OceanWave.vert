#version 330

// Vertex shader

// These variables are automatically assigned the value of each vertex and cooresponding normal and texcoord
// as they pass through the rendering pipeline. The layout locations are based on how the VAO was organized
layout (location = 0) in vec3 vertex_position;
layout (location = 1) in vec3 vertex_normal;
layout (location = 2) in vec2 vertex_texcoord;

// These are "global" variables that are set with a specific value within the application code using:
// shader.setUniform("view_mat", view);
// shader.setUniform("projection_mat", projection);
// shader.setUniform("model_mat", model);

uniform mat4 projection_mat, view_mat, model_mat;

// These "global" variables are marked as "out".  The out keyword means that they are shared
// between the vertex shader program and the fragment shader program.  You need to list each of these
// shared variables at the top of both your vertex and fragment programs.  The are "out" variables in the
// vertex shader and "in" variables in the fragment shader. The way they get shared 
// between the two programs is through an interpolation.  Since every fragment (another word for pixel)
// belongs to a triangle with 3 vertices, when OpenGl runs a fragment program it knows which triangle 
// the fragment belongs to and what the vertices of that triangle are.  If a variable is marked "out"
// then for each fragment OpenGl looks up the value of that variable in each of the 3 vertex programs run
// for the triangle, then it does a tri-linear interpolation of these 3 values (like a weighted average 
// based on how close the fragment is to each of the vertices) and uses this interpolated result for the
// value of the variable within the fragment program.  Since this value will vary across each pixel in 
// the triangle, in older versions of glsl these variables were called "varying" variables.


// Position of the current point on the surface, to be interpolated across the surface and passed to 
// the fragment program.
out vec4 interpSurfPosition;

// Normal of the current point on the surface, to be interpolated across the surface and passed to
// the fragment program.
out vec3 interpSurfNormal;


void main(void) {

	// vertex_position is a variable that holds the 3D position of the current vertex.  We want to
	// pass this position on to the fragment shader because we'll need it to calculate the lighting.  
	// We're also going to do one matrix multiplication at this stage in order to convert from object to world coordinates
    interpSurfPosition = model_mat * vec4 (vertex_position, 1.0);

	// We also need the normal to calculate lighting.  So, we will similarly pass it on to the fragment
	// program as an "out" variable, and we'll do the same type of matrix multiplication.  However,
	// it turns out you have to use a slightly different matrix for normals because they transform a 
	// bit differently than points.
	interpSurfNormal = mat3(transpose(inverse(model_mat))) * vertex_normal;


	// This is the last line of almost every vertex shader program.  We don't need this for our lighting
	// calculations, but it is required by OpenGl.  Whereas a fragment program must output a color
	// as its final result, a vertex program must output a vertex position that has been projected into the
	// 2D screen space as its final result.  The line below does this.  This is exactly what OpenGL
	// would do by default for us if we didn't write our own custom vertex program.
	gl_Position = projection_mat * view_mat * interpSurfPosition;
}
