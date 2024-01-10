#version 330 core

in vec3 f_color;
in vec2 texCoord;
out vec4 FragColor;

uniform sampler2D inputTexture;

vec3 convolution(vec2 uv, vec2 texelSize, mat3 kernel, sampler2D texture)
{
    vec3 result = vec3(0.0);

    for (int i = -1; i <= 1; ++i)
    {
        for (int j = -1; j <= 1; ++j)
        {
            vec2 offset = vec2(i, j) * texelSize;
            result += texture2D(texture, uv + offset).rgb * kernel[i + 1][j + 1];
        }
    }

    return result;
}

void main()
{
    vec2 texelSize = 1.0 / textureSize(inputTexture, 0);

    // Sobel operator kernels
    mat3 sobelX = mat3(-1, 0, 1, -2, 0, 2, -1, 0, 1);
    mat3 sobelY = mat3(-1, -2, -1, 0, 0, 0, 1, 2, 1);

    // Calculate gradients in x and y directions
    vec3 gradientX = convolution(texCoord, texelSize, sobelX, inputTexture);
    vec3 gradientY = convolution(texCoord, texelSize, sobelY, inputTexture);

    // Combine gradients to get edge magnitude
    float edgeMagnitude = length(gradientX) + length(gradientY);

    // Output edges as white color
    FragColor = vec4(vec3(edgeMagnitude), 1.0);
}