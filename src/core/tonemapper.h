class ToneMapper {
private:
    

public:
    static double luminance(vec3 colour) {
        return 0.299*colour.x + 0.587*colour.y + 0.114*colour.z;
        //return (colour.x+colour.x+colour.y+colour.y+colour.y+colour.z)/6;
    };
    static std::vector<std::vector<vec3>> reinhard(std::vector<std::vector<vec3>> radiance) {
        double totalLogLuminance = 0;
	    int nPixels = radiance.size()*radiance[0].size();
	    for (std::vector<vec3> column: radiance) {
            for (vec3 pixel : column) {
                double r = pixel.x;
                double g = pixel.y;
                double b = pixel.z;

                
                double luminance = ToneMapper::luminance(vec3(r,g,b));
                double logLuminance = log(1+luminance);
                totalLogLuminance += logLuminance;
            }
        }
        double averageLuminance = exp(totalLogLuminance/nPixels) - 1;
        double max = 0.0;
        for (int i = 0; i < radiance.size(); i++) {
            for (int j = 0; j < radiance[0].size(); j++) {
                double r = radiance[i][j].x;
                double g = radiance[i][j].y;
                double b = radiance[i][j].z;

                double lum = ToneMapper::luminance(vec3(r,g,b));
                double scaledLuminance = lum*0.2/averageLuminance;

                double finalLuminance = scaledLuminance/(1+scaledLuminance);

                if (lum == 0) {
                    radiance[i][j] = vec3();
                } else {
                    radiance[i][j] = vec3(r,g,b)*finalLuminance/lum;
                }
            }
        }
        for (int i = 0; i < radiance.size(); i++) {
            for (int j = 0; j < radiance[0].size(); j++) {
                radiance[i][j] = vec3(pow(radiance[i][j].x, 1/1.5), pow(radiance[i][j].y, 1/1.5), pow(radiance[i][j].z, 1/1.5));
            }
        }


        return std::vector<std::vector<vec3>>(radiance);
    };
    
};