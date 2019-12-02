//
//  Created by Ayush Kumar on 6/26/18.
//  Copyright © 2018 Ayush Kumar. All rights reserved.
//

#version 330 core
in vec3 vnormal;
in vec3 vpos;
in vec3 vlightpos;
in vec2 vuv;
flat in uint vmtl_idx;

uniform sampler2D diffuse_textures[12];
//uniform sampler2D specular_textures[11];

out vec4 color;

void main() {
    //we're working in the view space so the camera is at origin
    vec3 vcamerapos = vec3(0.0, 0.0, 0.0);
    vec3 normal = normalize(vnormal);
    vec3 cameradir = normalize(vcamerapos - vpos);
    vec3 lightdir = normalize(vlightpos - vpos);
    vec3 reflectdir = reflect(-lightdir, normal);
    vec3 h = (cameradir + lightdir) / length(cameradir + lightdir);
    float costheta = 0.4 * max(dot(lightdir, normal), 0.0);
    //blinn-phong
    float cosalpha = 0.3 * pow(max(dot(h, normal), 0.0), 32.0);
    
    vec3 tex_diff;
    //tex_diff = vec3(texture(diffuse_textures[vmtl_idx], vuv).rgb);
    //variable indexing into the sampler2D array is not allowed in GLSL 1.30 or higher, hence the above assignment is invalid
    switch(vmtl_idx) {
        case 0 : tex_diff = vec3(texture(diffuse_textures[0], vuv).rgb); break;
        case 1 : tex_diff = vec3(texture(diffuse_textures[1], vuv).rgb); break;
        case 2 : tex_diff = vec3(texture(diffuse_textures[2], vuv).rgb); break;
        case 3 : tex_diff = vec3(texture(diffuse_textures[3], vuv).rgb); break;
        case 4 : tex_diff = vec3(texture(diffuse_textures[4], vuv).rgb); break;
        case 5 : tex_diff = vec3(texture(diffuse_textures[5], vuv).rgb); break;
        case 6 : tex_diff = vec3(texture(diffuse_textures[6], vuv).rgb); break;
        case 7 : tex_diff = vec3(texture(diffuse_textures[7], vuv).rgb); break;
        case 8 : tex_diff = vec3(texture(diffuse_textures[8], vuv).rgb); break;
        case 9 : tex_diff = vec3(texture(diffuse_textures[9], vuv).rgb); break;
        case 10 : tex_diff = vec3(texture(diffuse_textures[10], vuv).rgb); break;
        case 11 : tex_diff = vec3(texture(diffuse_textures[11], vuv).rgb); break;
    }

    //vec3 tex_spec = vec3(texture(specular_textures[vmtl_idx], vuv).rgb);
    vec3 amb = 0.2 * tex_diff;
    vec3 diff = 2.0 * costheta * tex_diff;
    //vec3 spec = cosalpha * tex_spec;

    color = vec4(amb + diff, 1.0);
    //color = vec4(costheta, costheta, costheta, 1.0);
    return;
}