#version 330 core

/* shader info :
   *   memory total cost : 261 byte or more for echa vertex (as sizeof ourTexture) // not the best
   *       opengl default cost has not been calculated
   *   Normal and TexCoord and ourTexture are optional, but it still costs memory from VRAM/RAM
   *   you can reduce memory costs with replays the variables that use only ne time by there value
   *       viewDir, reflectDir : 24 byte save
*/

// get value from vertex shader
in  vec3 FragPos  ; //position                             // cost 12 byte
in  vec4 ourColor ; //color rgba                           // cost 16 byte
in  vec3 Normal   ; // normal                              // cost 12 byte
in  vec2 TexCoord ; // texture coords                      // cost  8 byte

out vec4 FragColor; // the final color that draw on screen // cost 16 byte

// light
uniform vec3 lightPos         = vec3(0,0,3)  ; // light position          // cost 12 byte
uniform vec4 lightColor       = vec4(1,1,1,1); // light color             // cost 16 byte
uniform float ambientStrength = 0.2f         ; // ambient light strength  // cost  4 byte
uniform float specularStrength = 0.5f        ; // specular light strength // cost  4 byte
// view
uniform vec3 viewPos          = vec3(0,0,3)  ; // camera position         // cost 12 byte
// texture
uniform sampler2D ourTexture                 ; // texture data            // cost undefine
uniform bool useTexture       = false        ; // if we use un texture    // cost  1 byte
// object color
uniform vec4 defaultColor     = vec4(-1,1,1,1); // default color             // cost 16 byte

void main() {
    vec3 ambient  = ambientStrength * lightColor.rgb               ; // get light color after apply ambient // cost 12 byte
    vec3 norm     = normalize(Normal)                              ; // normalize                           // cost 12 byte
    vec3 lightDir = normalize(lightPos - FragPos)                  ; // get light diraction                 // cost 12 byte
    vec3 diffuse  = max(dot(norm, lightDir), 0.0) * lightColor.rgb ; // diffuse apply on color              // cost 12 byte

    vec3 viewDir           = normalize(viewPos - FragPos)                 ; // cost 12 byte
    vec3 reflectDir        = reflect(-lightDir, norm)                     ; // cost 12 byte
    float spec             = pow(max(dot(viewDir, reflectDir), 0.0), 32)  ; // cost  4 byte
    vec3 specular          = specularStrength * spec * lightColor.rgb     ; // cost 12 byte

    vec3 result            = (ambient + diffuse + specular) * lightColor.a; // cost 12 byte

    if (useTexture)
        { FragColor = vec4(result, 1.0) * texture(ourTexture, TexCoord); } // cost 16 byte
    else if (defaultColor.r * defaultColor.g * defaultColor.b * defaultColor.a < 0)
        { FragColor = vec4(result, 1.0) * ourColor                     ; } // cost 16 byte
    else
        { FragColor = vec4(result, 1.0) * defaultColor                 ; } // cost 16 byte
}
