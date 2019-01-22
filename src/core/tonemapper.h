class ToneMapper {
private:
    static double luminance(Vector3 colour) {
        return (colour.x+colour.x+colour.y+colour.y+colour.y+colour.z)/6;
    };

public:
    static std::vector<std::vector<Vector3>> reinhard(std::vector<std::vector<Vector3>> radiance) {
        float totalLogLuminance = 0;
	    int nPixels = radiance.size()*radiance[0].size();
	    for (std::vector<Vector3> column: radiance) {
            for (Vector3 pixel : column) {
                double luminance = 1+ToneMapper::luminance(pixel);
                double logLuminance = log(luminance);
                totalLogLuminance += logLuminance;
            }
        }
        float averageLuminance = exp(totalLogLuminance/nPixels);

        for (int i = 0; i < radiance.size(); i++) {
            for (int j = 0; j < radiance[0].size(); j++) {
                
                float lum = ToneMapper::luminance(radiance[i][j]);
                float scaledLuminance = lum*0.3/averageLuminance;

                float finalLuminance = scaledLuminance/(1+scaledLuminance);

                if (lum == 0) {
                    radiance[i][j] = Vector3();
                } else {
                    radiance[i][j] = radiance[i][j]*finalLuminance/lum;
                }
                
            }
        }

        return std::vector<std::vector<Vector3>>(radiance);
    };
    
};