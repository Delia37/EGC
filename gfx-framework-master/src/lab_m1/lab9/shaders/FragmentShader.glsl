#version 330

// Input
in vec2 texcoord;

// Uniform properties
uniform sampler2D texture_1;
uniform sampler2D texture_2;
uniform float mix_factor;
uniform bool useMix;
uniform bool useTimeAnimation;
uniform float elapsedTime;

// Output
layout(location = 0) out vec4 out_color;

void main()
{
    // Calculate animated texture coordinates if time animation is enabled
    vec2 animatedTexcoord = texcoord;
    if (useTimeAnimation) {
        animatedTexcoord.x += mod(elapsedTime, 1.0); // Scroll texture horizontally
    }

    // Sample the first texture using the (potentially animated) texture coordinates
    vec4 color1 = texture(texture_1, animatedTexcoord);

    vec4 final_color;
    if (useMix) {
        // Sample the second texture and blend
        vec4 color2 = texture(texture_2, texcoord);
        final_color = mix(color1, color2, mix_factor);
    } else {
        // Use only the first texture
        final_color = color1;
    }

    // Perform alpha discard if the alpha component is below 0.5
    if (final_color.a < 0.5f) {
        discard;
    }

    // Set the final fragment color
    out_color = final_color;
}
