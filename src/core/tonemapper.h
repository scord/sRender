class ToneMapper {
private:
    static double luminance(Vector3 colour) {
        return (colour.x+colour.x+colour.y+colour.y+colour.y+colour.z)/6;
    };

public:
    static std::vector<std::vector<Vector3>> reinhard(std::vector<std::vector<Vector3>> radiance) {
        double totalLogLuminance = 0;
	    int nPixels = radiance.size()*radiance[0].size();
	    for (std::vector<Vector3> column: radiance) {
            for (Vector3 pixel : column) {
                double r = std::min(pixel.x, 1.0);
                double g = std::min(pixel.y, 1.0);
                double b = std::min(pixel.z, 1.0);

                
                double luminance = 1+ToneMapper::luminance(Vector3(r,g,b));
                double logLuminance = log(luminance);
                totalLogLuminance += logLuminance;
            }
        }
        double averageLuminance = exp(totalLogLuminance/nPixels);
        double max = 0.0;
        for (int i = 0; i < radiance.size(); i++) {
            for (int j = 0; j < radiance[0].size(); j++) {
                double r = std::min(radiance[i][j].x, 1.0);
                double g = std::min(radiance[i][j].y, 1.0);
                double b = std::min(radiance[i][j].z, 1.0);

                double lum = ToneMapper::luminance(Vector3(r,g,b));
                double scaledLuminance = lum*0.5/averageLuminance;

                double finalLuminance = scaledLuminance/(1+scaledLuminance);

                if (lum == 0) {
                    radiance[i][j] = Vector3();
                } else {
                    radiance[i][j] = Vector3(r,g,b)*finalLuminance/lum;
                }

                r = radiance[i][j].x;
                g = radiance[i][j].y;
                b = radiance[i][j].z;
                
                max = std::max(max, std::max(r, std::max(g, b)));
                
            }
        }
        for (int i = 0; i < radiance.size(); i++) {
            for (int j = 0; j < radiance[0].size(); j++) {
                radiance[i][j] = radiance[i][j]/max;
            }
        }


        return std::vector<std::vector<Vector3>>(radiance);
    };
    
};