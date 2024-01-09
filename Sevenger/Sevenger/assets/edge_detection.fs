#version 330 core

in vec2 TexCoord;
out vec4 FragColor;

uniform sampler2D inputTexture;

void main() {
    // Sobel Operator
    const float sobelX[9] = float[](-1, -2, -1, 0, 0, 0, 1, 2, 1);
    const float sobelY[9] = float[](-1, 0, 1, -2, 0, 2, -1, 0, 1);

    vec3 pixelColor = texture(inputTexture, TexCoord).rgb;
    vec3 resultX = vec3(0.0);
    vec3 resultY = vec3(0.0);

    // Apply Sobel operator
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            vec3 neighborColor = texture(inputTexture, TexCoord + vec2(i - 1, j - 1) / textureSize(inputTexture, 0)).rgb;
            int index = i * 3 + j;
            resultX += neighborColor * sobelX[index];
            resultY += neighborColor * sobelY[index];
        }
    }

    // Combine horizontal and vertical edge detection results
    vec3 edgeDetection = sqrt(resultX * resultX + resultY * resultY);

    // Set the result as the fragment color
    FragColor = vec4(edgeDetection, 1.0);
}