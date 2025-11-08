#version 330 core

/* shader info :
   *   memory total cost : 396 byte for echa vertex (as a buffer) // not the best
   *       opengl default cost has not been calculated
   *   Normal and TexCoord are optional, but it still costs memory from VRAM/RAM
*/

layout (location = 0) in vec2 aPos     ; // cost  8 byte
layout (location = 1) in vec4 aColor   ; // cost 16 byte
layout (location = 3) in vec3 aNormal  ; // cost 12 byte
layout (location = 4) in vec2 aTexCoord; // cost  8 byte
    
out    /*to fragment*/   vec3 FragPos  ; // cost 12 byte
out    /*to fragment*/   vec4 ourColor ; // cost 16 byte
out    /*to fragment*/   vec3 Normal   ; // cost 12 byte
out    /*to fragment*/   vec2 TexCoord ; // cost  8 byte
    
uniform mat4 model = mat4(1.0f); // cost 64 byte
uniform mat4 view              ; // cost 64 byte
uniform mat4 projection        ; // cost 64 byte
    
void main() {
    FragPos = vec3(model * vec4(aPos, 1.0, 1.0)); // cost 28 byte
    if (aNormal.x + aNormal.y + aNormal.z <= 0)
        { Normal = mat3(transpose(inverse(model))) * vec3(0, 0, 1); } // cost 48 byte 
    else
        { Normal = mat3(transpose(inverse(model))) * aNormal      ; } // cost 36 byte
    TexCoord = aTexCoord;
    ourColor = aColor   ;
        
    gl_Position = projection * view * vec4(FragPos, 1.0); // cost 16 byte
}
