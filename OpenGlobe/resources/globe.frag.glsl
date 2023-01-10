#version 450 core
#define M_PI 3.1415926535897932384626433832795
out vec4 FragColor;

in vec3 worldPosition;

uniform sampler2D u_texture;
uniform vec3 u_globeOneOverRadiiSquared;
uniform vec2 u_gridLineWidth;
uniform vec2 u_gridResolution;

const float og_oneOverTwoPi = 1.0 / (2 * M_PI);
const float og_oneOverPi = 1.0 / M_PI;


vec2 ComputeTextureCoordinates(vec3 normal)
{
    return vec2(atan(normal.x, normal.z) * og_oneOverTwoPi + 0.5,  asin(normal.y) * og_oneOverPi + 0.5);
}

vec3 GeodeticSurfaceNormal(vec3 positionOnEllipsoid, vec3 oneOverEllipsoidRadiiSquared)
{
    return normalize(positionOnEllipsoid * oneOverEllipsoidRadiiSquared);
}

void main()
{
    vec3 normal = GeodeticSurfaceNormal(worldPosition, u_globeOneOverRadiiSquared);
    vec2 textureCoordinate = ComputeTextureCoordinates(normal);
    vec2 distanceToLine = mod(textureCoordinate, u_gridResolution);
    vec2 dx = abs(dFdx(textureCoordinate));
    vec2 dy = abs(dFdy(textureCoordinate));
    vec2 dF = vec2(max(dx.s, dy.s), max(dx.t, dy.t)) * u_gridLineWidth;

    if (any(lessThan(distanceToLine, dF)))
    {
        FragColor = vec4(1.0f, 0, 0, 1.0f);
    }
    else
    {
        FragColor = texture(u_texture, textureCoordinate);
    }
}