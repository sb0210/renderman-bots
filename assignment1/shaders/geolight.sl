class geolight (float intensity = 1;
                color lightColor = 1;
                float finalOpacity = 1)
{
    public void prelighting (output color Ci, Oi)
    {
        color emission = intensity * lightColor;
        Oi = finalOpacity;
        emit(emission, "specularcontribution", 1, "diffusecontribution", 1);
    }

    public void surface(output color Ci, Oi) {
        Ci = intensity * lightColor;
        Oi = finalOpacity;
    }
}