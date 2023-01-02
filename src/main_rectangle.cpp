//////////////////////////////////////////////////////////////////////////
// Option Images
// Projet, s�ance 1
// th�me : premier pas en OpenCV
// contenu : charge, affiche, r�duction, calcul et affichage d'histogramme
// version : 17.1128
//////////////////////////////////////////////////////////////////////////


#include "main_rectangle.h"

std::string fixedDigitInt(int input, int digits) {
    std::stringstream ss;
    ss << std::setw(digits) << std::setfill('0') << input;
    return ss.str();
}

std::string detectSize(Mat &subImage) {
    vector<cv::String> size_templ;
    glob("../size_template/*.png", size_templ, false);
    vector<Mat> size_templates;
    Mat sub_img_gray;
    Mat res_sub;
    cvtColor(subImage, sub_img_gray, COLOR_BGR2GRAY);
    int res_size = -1;
    for (size_t j = 0; j < size_templ.size(); j++) {
        size_templates.push_back(imread(size_templ[j], IMREAD_GRAYSCALE));
        try {
            matchTemplate(sub_img_gray, size_templates[j], res_sub, TM_CCORR_NORMED);
            threshold(res_sub, res_sub, 0.1, 1., THRESH_TOZERO);
            double minval2, maxval2;
            double threshold2 = 0.99;
            Point minloc2, maxloc2;
            minMaxLoc(res_sub, &minval2, &maxval2, &minloc2, &maxloc2);
            if (maxval2 >= threshold2) {
                res_size = j;
            }
        } catch (cv::Exception &exception) {

        }
    }
    string size="";
    switch (res_size){
        case -1 :
            break;
        case 0 :
            size=" large";
            break;
        case 1 :
            size=" medium";
            break;
        case 2 :
            size=" small";
    }
    cout << "taille :" << size << endl;
    return size;
}


int guessRow(int y) {
    // Pour savoir sur quelle ligne on est, on regarde simplement sur un intervalle et puis ItWorks(TM)
    if (y>2466) return 7;
    if (y>2133) return 6;
    if (y>1770) return 5;
    if (y>1430) return 4;
    if (y>1080) return 3;
    if (y>740) return 2;
    return 1;
}

bool parse_page(const std::string &imName, int scripterNb, int pageNb) {
    Mat img_rgb;
    if (!rectify(imName, img_rgb))
        return false;
    Mat img_gray;
    cvtColor(img_rgb, img_gray, COLOR_BGR2GRAY);
    Mat shape_template = imread("../symboles/car.png", 0);

    Mat img_display;
    img_rgb.copyTo(img_display);

    Mat res;
    Point matchLoc;


    vector<cv::String> symb;
    glob("../symboles/*.png", symb, false);
    vector<Mat> symboles;


    size_t count = symb.size();
    for (size_t i = 0; i < count; i++) {
        std::cout << "symbole testé : " << symb[i] << std::endl;
        symboles.push_back(imread(symb[i], IMREAD_GRAYSCALE));
        bool parsed[] = {false, false, false, false, false, false, false};
        try {
            matchTemplate(img_gray, symboles[i], res, TM_CCOEFF_NORMED);
            threshold(res, res, 0.1, 1., THRESH_TOZERO);
            while (true) {
                double minval, maxval;
                double thresh = 0.55; //Pour le symbole electricity (i=4), il reconnait parfois la croix en bas comme étant le symbole electricity donc augmenter le seuil pour i=4
                Point minloc, maxloc;
                minMaxLoc(res, &minval, &maxval, &minloc, &maxloc);
                matchLoc = maxloc;

                if (maxval >= thresh) {
                    int row = guessRow(matchLoc.y - 120);
                    floodFill(res, maxloc, 0); //mark drawn blob
                    if (parsed[row-1])
                        continue; // Pour ne pas refaire le traitement d'une même ligne
                    parsed[row-1] = true;

                    Rect crop(matchLoc.x - 40, matchLoc.y - 120, 2100, 360);
                    Mat sub_image = img_display(crop);
                    string size = detectSize(sub_image);

                    //imshow(  "test " + symb[i], sub_image);
//                    i--; //pour retester le meme objet si on l'a déjà trouvé, mais il faut dans ce cas que je genere une sous image ne contenant pas la partie extraite puis refaire l'opération dessus //TODO
                    //Extract thumbnails from line
                    string prefix = "../output/" + symb[i].substr(12, symb[i].size());
                    prefix.erase(prefix.size()-4, 4);
                    prefix+="_"+ fixedDigitInt(scripterNb, 3);
                    prefix+="_"+ fixedDigitInt(pageNb, 2);
                    prefix+="_"+ fixedDigitInt(row,1);
                    prefix+="_";
                    mainExtractThumbnails(sub_image, prefix, size); //prendre en compte le sizeSymbol une fois la détection de la taille implémentée
                }
                else
                {
                    cout << "\nTemplate does not match with input image\n";
                    break;
                }
            }
        }catch(cv::Exception &exception){
            std::cerr << "symbole N°" << i << " not found" << std::endl;
        }
    }
    return true;
}
