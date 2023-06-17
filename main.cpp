#include <iostream>
#include <Windows.h> // for file dialog(choosing file), getting screen size
#include <string>
#include <vector>
#include <cmath>
#include <ctime>

#include <stdio.h>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>

using namespace std;
/*
Explanations about things here:
classes: when creating the class the first letter will be uppercase, when defining a variable it will be like this: cClassName
class variable look: cClassName
functions: the funtions(not in the classes) name will be with the first letter of what it returns, example of a function that returns int: nReturnN, void: vPrint
functions: the ones that are used in a specific class will be before the class
function definition look: returnTypeFirstLetter+funcName: int-iGetNum()
*/


/*
to do list:

To add:

To fix:
*/

// circle functions
void drawCircle_NotFilled(SDL_Renderer* renderer, int x0, int y0, int radius, int r, int g, int b, int a) {
    SDL_SetRenderDrawColor(renderer, r, g, b, a);
    int x = radius - 1;
    int y = 0;
    int dx = 1;
    int dy = 1;
    int err = dx - (radius << 1);

    while (x >= y)
    {
        SDL_RenderDrawPoint(renderer, x0 + x, y0 + y);
        SDL_RenderDrawPoint(renderer, x0 + y, y0 + x);
        SDL_RenderDrawPoint(renderer, x0 - y, y0 + x);
        SDL_RenderDrawPoint(renderer, x0 - x, y0 + y);
        SDL_RenderDrawPoint(renderer, x0 - x, y0 - y);
        SDL_RenderDrawPoint(renderer, x0 - y, y0 - x);
        SDL_RenderDrawPoint(renderer, x0 + y, y0 - x);
        SDL_RenderDrawPoint(renderer, x0 + x, y0 - y);

        if (err <= 0)
        {
            y++;
            err += dy;
            dy += 2;
        }

        if (err > 0)
        {
            x--;
            dx += 2;
            err += dx - (radius << 1);
        }
    }
}
/*
for the next draw circle functions:
w < radius: right side of the circle
w > radius: left side of the circle
h < radius: down side of the circle
h > radius: up side of the circle
it draws the circle with lines, draw the pixels in a vertical(up-down) line
*/
void drawCircle_Filled(SDL_Renderer* renderer, int x, int y, int radius, int r, int g, int b, int a)
{
    SDL_SetRenderDrawColor(renderer, r, g, b, a);
    for (int w = 0; w < radius * 2; w++)
    {
        for (int h = 0; h < radius * 2; h++)
        {
            int dx = radius - w; // horizontal offset
            int dy = radius - h; // vertical offset
            if ((dx * dx + dy * dy) <= (radius * radius))
            {
                SDL_RenderDrawPoint(renderer, x + dx, y + dy);
            }
        }
    }
}
void drawCircle_SprayMode(SDL_Renderer* renderer, int x, int y, int radius, int r, int g, int b, int a)
{
    // draws every second pixel
    SDL_SetRenderDrawColor(renderer, r, g, b, a);
    int n = 0;

    for (int w = 0; w < radius * 2; w++)
    {
        for (int h = 0; h < radius * 2; h++)
        {
            int dx = radius - w; // horizontal offset
            int dy = radius - h; // vertical offset
            if ((dx * dx + dy * dy) <= (radius * radius))
            {
                if (n == 2) {
                    SDL_RenderDrawPoint(renderer, x + dx, y + dy);
                    n = 0;
                }
                n += 1;
            }
        }
    }
}
void drawCircle_SprayMode2(SDL_Renderer* renderer, int x, int y, int radius, int r, int g, int b, int a)
{
    SDL_SetRenderDrawColor(renderer, r, g, b, a);
    for (int w = 0; w < radius * 2; w++)
    {
        for (int h = 0; h < radius * 2; h++)
        {
            int dx = radius - w; // horizontal offset
            int dy = radius - h; // vertical offset
            if ((dx * dx + dy * dy) <= (radius * radius))
            {
                if (w % 2 == 0 && h % 2 != 0) {
                    SDL_RenderDrawPoint(renderer, x + dx, y + dy);
                }
            }
        }
    }
}
void drawCircle_Faded(SDL_Renderer* renderer, int x, int y, int radius, int r, int g, int b, int a)
{
    SDL_SetRenderDrawColor(renderer, r, g, b, a);
    int a2 = 0;
    for (int w = 0; w < radius * 2; w++)
    {
        for (int h = 0; h < radius * 2; h++)
        {
            int dx = radius - w; // horizontal offset
            int dy = radius - h; // vertical offset
            if ((dx * dx + dy * dy) <= (radius * radius))
            {
                a2 += 1;
                SDL_SetRenderDrawColor(renderer, r, g, b, a2);
                SDL_RenderDrawPoint(renderer, x + dx, y + dy);
            }
        }
    }
}
// classes for the mouse
class Mouse {
    public:
        int x, y;
        bool showMouse = true; // false when drawing ane needing to see what is behind and stuff
        void vUpdateMousePos() {
            //SDL_GetGlobalMouseState(&this->x, &this->y); // get the mouse position IN THE SCREEN NOT THE WINDOW
            SDL_GetMouseState(&this->x, &this->y);
            //cout << "Mouse pos, x:" << this->x << " y:" << this->y << endl;
        }
};
class MouseCircle {
    public:
        int radius = 5;
        bool leftMouseButtonPressed = false;
        void draw(Mouse m, SDL_Renderer* renderer, int r,int g,int b, int a) {
            if (this->leftMouseButtonPressed == true) {
                drawCircle_Filled(renderer, m.x, m.y, this->radius, r-50,g-50,b-50,a-100);
                //drawCircle_Filled(renderer, m.x, m.y, this->radius, 100, 100, 100, 100);
                drawCircle_NotFilled(renderer, m.x, m.y, this->radius, 50,50,50, 150); // for better effect
            } else {
                drawCircle_Filled(renderer, m.x, m.y, this->radius, 100,100,100,50); // for when we have a hard color to see
                if (r >= 200 && g >= 200 && b >= 200 || a <= 200) { // if it will be hard to see the mouse it will draw it in black
                    drawCircle_NotFilled(renderer, m.x, m.y, this->radius, 0,0,0, 255);
                } else {
                    drawCircle_NotFilled(renderer, m.x, m.y, this->radius, r, g, b, 255);
                }
            }
        }
};

// helpful functions
// create specific sdl object that require more then 1 line
SDL_Rect srCreateRectObject(int x, int y, int width, int height) {
    // because the SDL_Rect is a structure and you can't define it in one line this function creates it for you for not having to do the same 5 lines over and over
    SDL_Rect rect;
    rect.x = x;
    rect.y = y;
    rect.w = width;
    rect.h = height;
    return rect;
}
SDL_Color scCreateSDL_ColorVariable(int r, int g, int b, int a) {
    SDL_Color color = { r,g,b,a };
    return color;
}
// blit text function(display text on screen)
vector<int> blitText(int x, int y, SDL_Color color, string text, TTF_Font* f, SDL_Renderer* r) {
    string str(text);
    SDL_Color c = { 0,0,0 ,255 }; // color of the text
    SDL_Surface* fontSurface = TTF_RenderText_Solid(f, str.c_str(), c);
    SDL_Texture* fontTexture = SDL_CreateTextureFromSurface(r, fontSurface);
    SDL_Rect fontRect = { x, y, fontSurface->w,fontSurface->h };
    SDL_RenderCopy(r, fontTexture, NULL, &(fontRect));
    vector<int> v = {x,y,fontSurface->w,fontSurface ->h};
    return v;
}
// blit image function(display image on screen)
void blitImage(SDL_Renderer* r, string path, int x, int y, int w, int h) {
    string str(path);
    SDL_Surface* image = IMG_Load(str.c_str());
    SDL_Texture* imageTexture = SDL_CreateTextureFromSurface(r, image);
    SDL_Rect imageRect = srCreateRectObject(x, y, w, h);
    SDL_RenderCopy(r, imageTexture, NULL, &(imageRect));
}
// open file dialog that lets the user choose file
string openFileDialogToOpenFile() {
    // as long as the file is in a folder with english letters it will work
    OPENFILENAMEW ofn;
    wchar_t fileName[MAX_PATH] = L"";

    ZeroMemory(&ofn, sizeof(ofn));
    ofn.lStructSize = sizeof(ofn);
    ofn.hwndOwner = NULL;
    ofn.lpstrFilter = L"PNG Files (*.png)\0*.png\0JPEG Files (*.jpg;*.jpeg)\0*.jpg;*.jpeg\0All Files (*.*)\0*.*\0"; // png/jpg   // L"All Files (*.*)\0*.*\0";
    ofn.lpstrFile = fileName;
    ofn.nMaxFile = MAX_PATH;
    ofn.Flags = OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST;

    if (GetOpenFileNameW(&ofn)) {
        char fileNameMb[MAX_PATH];
        WideCharToMultiByte(CP_ACP, 0, fileName, -1, fileNameMb, MAX_PATH, NULL, NULL);
        string filePath(fileNameMb);
        cout << "Selected file path: " << filePath << std::endl;
        return filePath;
    }
    return "";
}
string openFileDialogToSaveFile() {
    OPENFILENAMEW ofn;
    wchar_t fileName[MAX_PATH] = L"draw";

    ZeroMemory(&ofn, sizeof(ofn));
    ofn.lStructSize = sizeof(ofn);
    ofn.hwndOwner = NULL;
    ofn.lpstrFilter = L"PNG Files (*.png)\0*.png\0JPEG Files (*.jpg;*.jpeg)\0*.jpg;*.jpeg\0All Files (*.*)\0*.*\0"; // png/jpg
    ofn.lpstrFile = fileName;
    ofn.nMaxFile = MAX_PATH;
    ofn.Flags = OFN_OVERWRITEPROMPT;

    if (GetSaveFileNameW(&ofn)) {
        char fileNameMb[MAX_PATH];
        WideCharToMultiByte(CP_ACP, 0, fileName, -1, fileNameMb, MAX_PATH, NULL, NULL);
        string filePath(fileNameMb);
        cout << "Selected file path: " << filePath << std::endl;
        return filePath;
    }
    return "";
}
// a function that checks circle collision
bool checkCirclePointCollision(int x1, int y1, int r, int x2, int y2) {
    if (sqrt(pow(x1 - x2, 2) + pow(y1 - y2, 2)) <= r) {
        return true;
    }
    return false;
}
bool checkCirclesCollision(int x1, int y1, int r1, int x2, int y2, int r2) {
    if (  sqrt(pow(x1 - x2, 2) + pow(y1 - y2, 2)) <= r1 + r2  ) {
        return true;
    }
    return false;
}
bool checkRectsCollision(SDL_Rect r1, SDL_Rect r2) {
    int x1 = r1.x;
    int y1 = r1.y;
    int width1 = r1.w;
    int height1 = r1.h;
    int x2 = r2.x;
    int y2 = r2.y;
    int width2 = r2.w;
    int height2 = r2.h;
    if (x1 <= x2 && x1 + width1 >= x2 || x2 <= x1 && x2 + width2 >= x1) {
        if (y1 <= y2 && y1 + height1 >= y2 || y2 <= y1 && y2 + height2 >= y1) {
            return true;
        }
    }
    return false;
}
bool checkCircleLineCollision(float centerX, float centerY, float radius, float lineStartX, float lineStartY, float lineEndX, float lineEndY) {
    // Calculate the distance between the circle center and the line segment
    float dx = lineEndX - lineStartX;
    float dy = lineEndY - lineStartY;
    float lineLength = sqrt(dx * dx + dy * dy);

    float u = ((centerX - lineStartX) * dx + (centerY - lineStartY) * dy) / (lineLength * lineLength);

    // Find the closest point on the line segment to the circle center
    float closestX, closestY;
    if (u < 0) {
        closestX = lineStartX;
        closestY = lineStartY;
    }
    else if (u > 1) {
        closestX = lineEndX;
        closestY = lineEndY;
    }
    else {
        closestX = lineStartX + u * dx;
        closestY = lineStartY + u * dy;
    }

    // Check if the closest point is within the circle
    float distance = sqrt((closestX - centerX) * (closestX - centerX) + (closestY - centerY) * (closestY - centerY));
    return distance <= radius;
}

void takeAScreenShot(SDL_Surface* surface,SDL_Renderer* r,string fileName, int startX, int startY, int width, int height) {
    SDL_Rect rectToCapture = { startX, startY, width, height }; // Specify the rectangle to capture

    // Create a surface to hold the captured pixels
    SDL_Surface* captureSurface = SDL_CreateRGBSurface(0, rectToCapture.w, rectToCapture.h, 32, 0, 0, 0, 0);

    // Copy the pixels within the rectangle from the renderer to the capture surface
    SDL_RenderReadPixels(r, &rectToCapture, SDL_PIXELFORMAT_ARGB8888, captureSurface->pixels, captureSurface->pitch);

    // Save the captured surface to an image file
    fileName = fileName + ".png";
    string fileName2(fileName);
    IMG_SavePNG(captureSurface, fileName2.c_str());
}

// classes for the buttons in the app
class Button {
    //not using the blit text function here because we dont need it to create the variables each time again, it will just do it in the constructor
    public:
        int x, y; // positions of the button
        int width, height;
        SDL_Color color; // color of the button
        string text; // the text that will be on the button
        int h = 100;

        SDL_Renderer* renderer;
        TTF_Font* font;
        SDL_Surface* fontSurface;
        SDL_Texture* fontTexture;
        SDL_Rect fontRect;
        Button(int x, int y, SDL_Color color, string text, TTF_Font* f,SDL_Renderer* r) {
            this->x = x;
            this->y = y;
            this->color = color;
            this->text = text;
            this->font = f;
            this->renderer = r;

            string str(text);
            SDL_Color c = { 0,0,0 ,255}; // color of the text
            this->fontSurface = TTF_RenderText_Solid(this->font, str.c_str(), c);
            this->fontTexture = SDL_CreateTextureFromSurface(r, this->fontSurface);
            // adding 5,1 to the text for asthetics
            this->fontRect = { x+5, y-1, this->fontSurface->w, this->fontSurface->h };

            // adding 5,1 to the width,height for astethics and fitting it with the x+5,y+1
            this->width = 5 + this->fontSurface->w + 5;
            this->height = 1 + this->fontSurface->h + 1;
        }
        Button() { // a constructor for when creating a few buttons and wanting to do btn = Button() each time

        }
        bool checkIfMouseOnBtn(Mouse m) {
            if (m.x > this->x && m.x < this->x + this->width) {
                if (m.y > this->y && m.y < this->y + this->height) {
                    return true;
                }
            }
            return false;
        }
        void draw(Mouse m) {
            bool drew = false;
            if (this -> checkIfMouseOnBtn(m) == true) {
                this->drawWhenHovering();
                drew = true;
            }
            if (drew == false){
                this->drawWhenNotHovering();
            }
        }
        void drawWhenNotHovering() {
            // draw a rect of the button
            SDL_Rect rect = srCreateRectObject(this->x, this->y,this->width,this->height);
            SDL_SetRenderDrawColor(this->renderer, this->color.r, this->color.g, this->color.b, this->color.a); // setting the color of the inner rect
            SDL_RenderFillRect(this->renderer, &rect); // draw the filled in rect(inner rect)
            SDL_SetRenderDrawColor(this->renderer, this->color.r - 50, this->color.g - 50, this->color.b - 50, this->color.a); // setting the color of the outer rect(for better looking)
            SDL_RenderDrawRect(this->renderer, &rect); // draw the outline rect(outer rect)
            
            // draw the text on it
            SDL_RenderCopy(this->renderer, this->fontTexture, NULL, &(this->fontRect));
        }
        void drawWhenHovering() {
            // draw a rect of the button
            SDL_Rect rect = srCreateRectObject(this->x, this->y, this->width, this->height);
            SDL_SetRenderDrawColor(this->renderer, this->color.r-50, this->color.g-50, this->color.b-50, this->color.a); // setting the color of the inner rect
            SDL_RenderFillRect(this->renderer, &rect); // draw the filled in rect(inner rect)
            SDL_SetRenderDrawColor(this->renderer, abs(this->color.r - 50 - 50), abs(this->color.g - 50 - 50), abs(this->color.b - 50 - 50), this->color.a); // setting the color of the outer rect(for better looking)
            SDL_RenderDrawRect(this->renderer, &rect); // draw the outline rect(outer rect)

            // draw the text on it
            SDL_RenderCopy(this->renderer, this->fontTexture, NULL, &(this->fontRect));
        }
};
class Buttons {
    public:
        vector<Button> buttons;
        Buttons(vector<Button> v) { // if wanting to add an existing vector of buttons
            for (Button b : v) {
                buttons.push_back(b);
            }
        }
        Buttons(vector<SDL_Renderer*> renderers,vector<string> buttonsText, vector<SDL_Color> buttonsColors,vector<TTF_Font*> fonts, int buttonX, int buttonY) {
            SDL_Color color;
            Button btn; // defualt one
            string btnText;
            for (int i = 0; i < buttonsText.size(); i++) {
                btn = Button(buttonX,buttonY, buttonsColors.at(i), buttonsText.at(i), fonts.at(i), renderers.at(i));
                this->buttons.push_back(btn);
                buttonX += 1 + btn.width; // the 1 is for space between the buttons
            }
        }
        void addButton(string btnText, SDL_Color color, int x, int y, TTF_Font* f, SDL_Renderer* r) {
            Button b = Button(x, y, color, btnText,f,r);
            this->buttons.push_back(b);
        }
};

// choosing color class
class ColorPicker {
    // this is the class of the color picker(pressing on the "Color Picker" button and this will control what is showing
    /*
    ideas of how it will look:
    a rect with all of the options(can press on the color it wants)(similar to the one word have)
    3 lines, red green blue, you choose the value of each and the current color is showing below
    */
    public:
        SDL_Renderer* renderer;
        int x, y;
        int r, g, b, a;
        int width = 400;
        int height = width;
        int eachColorInLineWidth = (width-50) / 255;
        int eachColorInLineHeight = 10;

        SDL_Rect WindowRect;
        Button closeScreenBtn;

        TTF_Font* font;
        SDL_Surface* fontSurface;
        SDL_Texture* fontTexture;
        SDL_Rect fontRect;
        ColorPicker(SDL_Renderer* renderer, int r,int g,int b, int screenWidth1, int screenHeight1,TTF_Font* f) {
            this->renderer = renderer;
            this->r = r;
            this->g = g;
            this->b = b;
            this->a = 255;
            this->x = screenWidth1 / 2 - this->width / 2;
            this->y = screenHeight1 / 2 - this->height / 2;
            this->WindowRect = srCreateRectObject(this->x, this->y, this->width, this->height);
            this->closeScreenBtn = Button(this->x, this->y, scCreateSDL_ColorVariable(238, 59, 59, 255),"X",f,renderer);

            this->font = f;
            this->fontSurface = TTF_RenderText_Solid(f, "Color Picker", {0,0,0,255});
            this->fontTexture = SDL_CreateTextureFromSurface(renderer, fontSurface);
            this->fontRect = { x+this->width/2-fontSurface->w/2, y+5, fontSurface->w,fontSurface->h };
        }
        void draw(Mouse m) {
            // draw the background/window(what the colors will be on
            SDL_SetRenderDrawColor(this->renderer, 200,200,200,200); // setting the color of the inner rect
            SDL_RenderFillRect(this->renderer, &(this->WindowRect)); // draw the filled in rect(inner rect)
            SDL_SetRenderDrawColor(this->renderer, 100, 100,100,150); // setting the color of the outer rect(for better looking)
            SDL_RenderDrawRect(this->renderer, &(this->WindowRect)); // draw the outline rect(outer rect)
            // draw the buttons
            this->closeScreenBtn.draw(m);
            // draw the text above("Color Picker")
            SDL_RenderCopy(this->renderer, this->fontTexture, NULL, &(this->fontRect));
            // draw the actual colorPicker

            int startX = this->x + this->width / 2 - this->eachColorInLineWidth * 255 / 2;
            SDL_Rect eachValueRect;
            // red line
            int x = startX;
            int y = this->y + this->height / 5;
            int onColorX, onColorY;
            for (int i = 0; i <= 255; i++) {
                eachValueRect = srCreateRectObject(x, y, this->eachColorInLineWidth, this->eachColorInLineHeight);
                SDL_SetRenderDrawColor(this->renderer, i, 0, 0, 255); // setting the color of the outer rect(for better looking)
                SDL_RenderFillRect(this->renderer, &(eachValueRect)); // draw the filled in rect
                if (i == this->r) {
                    onColorX = x;
                    onColorY = y;
                }
                x += this->eachColorInLineWidth;
            }
            eachValueRect = srCreateRectObject(onColorX, onColorY - 3, 3, this->eachColorInLineHeight + 6);
            SDL_SetRenderDrawColor(this->renderer, 0,0,0, 255); // setting the color of the outer rect(for better looking)
            SDL_RenderFillRect(this->renderer, &(eachValueRect)); // draw the filled in rect
            // green line
            x = startX;
            y += this->eachColorInLineHeight * 3;
            for (int i = 0; i <= 255; i++) {
                eachValueRect = srCreateRectObject(x, y, this->eachColorInLineWidth, this->eachColorInLineHeight);
                SDL_SetRenderDrawColor(this->renderer, 0, i, 0, 255); // setting the color of the outer rect(for better looking)
                SDL_RenderFillRect(this->renderer, &(eachValueRect)); // draw the filled in rect
                if (i == this->g) {
                    onColorX = x;
                    onColorY = y;
                }
                x += this->eachColorInLineWidth;
            }
            eachValueRect = srCreateRectObject(onColorX, onColorY - 3, 3, this->eachColorInLineHeight + 6);
            SDL_SetRenderDrawColor(this->renderer, 0, 0, 0, 255); // setting the color of the outer rect(for better looking)
            SDL_RenderFillRect(this->renderer, &(eachValueRect)); // draw the filled in rect
            // blue line
            x = startX;
            y += this->eachColorInLineHeight * 3;
            for (int i = 0; i <= 255; i++) {
                eachValueRect = srCreateRectObject(x, y, this->eachColorInLineWidth, this->eachColorInLineHeight);
                SDL_SetRenderDrawColor(this->renderer, 0, 0, i, 255); // setting the color of the outer rect(for better looking)
                SDL_RenderFillRect(this->renderer, &(eachValueRect)); // draw the filled in rect
                if (i == this->b) {
                    onColorX = x;
                    onColorY = y;
                }
                x += this->eachColorInLineWidth;
            }
            eachValueRect = srCreateRectObject(onColorX, onColorY - 3, 3, this->eachColorInLineHeight + 6);
            SDL_SetRenderDrawColor(this->renderer, 0, 0, 0, 255); // setting the color of the outer rect(for better looking)
            SDL_RenderFillRect(this->renderer, &(eachValueRect)); // draw the filled in rect

            // alpha level line
            x = startX;
            y += this->eachColorInLineHeight * 3;
            for (int i = 0; i <= 255; i++) {
                eachValueRect = srCreateRectObject(x, y, this->eachColorInLineWidth, this->eachColorInLineHeight);
                SDL_SetRenderDrawColor(this->renderer, 0, 0, 255, i); // setting the color of the outer rect(for better looking)
                SDL_RenderFillRect(this->renderer, &(eachValueRect)); // draw the filled in rect
                if (i == this->a) {
                    onColorX = x;
                    onColorY = y;
                }
                x += this->eachColorInLineWidth;
            }
            eachValueRect = srCreateRectObject(onColorX, onColorY - 3, 3, this->eachColorInLineHeight + 6);
            SDL_SetRenderDrawColor(this->renderer, 0, 0, 0, 255); // setting the color of the outer rect(for better looking)
            SDL_RenderFillRect(this->renderer, &(eachValueRect)); // draw the filled in rect
            
            // drawing the values of r,g,b
            y += this->eachColorInLineHeight * 3;
            blitText(this->x+5, y, { 0,0,0,255 }, "Color Values:", this->font, this->renderer);
            y += this->eachColorInLineHeight * 3;
            string text = "red-" + to_string(this->r) + " green-" + to_string(this->g) + " blue-" + to_string(this->b) + " alpha level-" + to_string(this->a);
            blitText(this->x+5,y,{0,0,0,255},text,this->font,this->renderer);

            // drawing the actual color as a rect(a square)
            y += this->eachColorInLineHeight * 3;
            vector<int> textInfo = blitText(this->x + 5, y, { 0,0,0,255 }, "Color:", this->font, this->renderer);
            eachValueRect = srCreateRectObject(this->x+5+textInfo.at(2)+5, y, 50, 50);
            SDL_SetRenderDrawColor(this->renderer, this->r,this->g,this->b, this->a);
            SDL_RenderFillRect(this->renderer, &(eachValueRect)); // draw the filled in rect
            SDL_SetRenderDrawColor(this->renderer, 0,0,0, 255);
            SDL_RenderDrawRect(this->renderer, &(eachValueRect)); // draw the filled in rect
        }
        void handleMouse(Mouse m) {
   
            int startX = this->x + this->width / 2 - this->eachColorInLineWidth * 255 / 2;

            // red line
            int x = startX;
            int y = this->y + this->height / 5;
            for (int i = 0; i <= 255; i++) {
                if (x <= m.x && x + this->eachColorInLineWidth > m.x) {
                    if (y < m.y && y + this->eachColorInLineHeight > m.y) {
                        this->r = i;
                    }
                }
                x += this->eachColorInLineWidth;
            }
            // green line
            x = startX;
            y += this->eachColorInLineHeight * 3;
            for (int i = 0; i <= 255; i++) {
                if (x <= m.x && x + this->eachColorInLineWidth > m.x) {
                    if (y < m.y && y + this->eachColorInLineHeight > m.y) {
                        this->g = i;
                    }
                }
                x += this->eachColorInLineWidth;
            }
            // blue line
            x = startX;
            y += this->eachColorInLineHeight * 3;
            for (int i = 0; i <= 255; i++) {
                if (x <= m.x && x + this->eachColorInLineWidth > m.x) {
                    if (y < m.y && y + this->eachColorInLineHeight > m.y) {
                        this->b = i;
                    }
                }
                x += this->eachColorInLineWidth;
            }
            // blue line
            x = startX;
            y += this->eachColorInLineHeight * 3;
            for (int i = 0; i <= 255; i++) {
                if (x <= m.x && x + this->eachColorInLineWidth > m.x) {
                    if (y < m.y && y + this->eachColorInLineHeight > m.y) {
                        this->a = i;
                    }
                }
                x += this->eachColorInLineWidth;
            }
        }
        bool pressedToClosePicker(Mouse m) { // checking only when the user presses the left mouse button
            // return true when:
            // the user pressed the close button
            // the user pressed something outside the screen
            return this->closeScreenBtn.checkIfMouseOnBtn(m) || // check the button
                (m.x < this->x || m.x > this->x+this->width || m.y < this->y || m.y > this->y + this->height); // check if pressed on anything outside the window
        }
};
class DrawModePicker {
    public:
        SDL_Renderer* renderer;
        int closeBtnX, closeBtnY;
        int drawingButtonsStartXOriginal, drawingButtonsStartYOriginal; // for when need to reset the two variables
        int drawingButtonsStartX, drawingButtonsStartY;
        int width = 400;
        int height = width;
        int circlesRadius = 20;

        string drawMode = "filled"; // filled/spray/spray2/faded

        SDL_Rect WindowRect;
        Button closeScreenBtn;

        TTF_Font* font;
        // for the headline of this mini window
        SDL_Surface* fontSurface;
        SDL_Texture* fontTexture;
        SDL_Rect fontRect;
        DrawModePicker(SDL_Renderer* renderer, int screenWidth1, int screenHeight1, TTF_Font* f) {
            this->renderer = renderer;
            this->closeBtnX = screenWidth1 / 2 - this->width / 2;
            this->closeBtnY = screenHeight1 / 2 - this->height / 2;
            this->drawingButtonsStartX = this->closeBtnX + 30;
            this->drawingButtonsStartXOriginal = this->drawingButtonsStartX;
            this->drawingButtonsStartY = this->closeBtnY + this->width / 3;
            this->drawingButtonsStartYOriginal = this->drawingButtonsStartY;
            this->WindowRect = srCreateRectObject(this->closeBtnX, this->closeBtnY, this->width, this->height);
            this->closeScreenBtn = Button(this->closeBtnX, this->closeBtnY, scCreateSDL_ColorVariable(238, 59, 59, 255), "X", f, renderer);

            this->font = f;
            this->fontSurface = TTF_RenderText_Solid(f, "Drawing Modes Picker", { 0,0,0,255 });
            this->fontTexture = SDL_CreateTextureFromSurface(renderer, fontSurface);
            this->fontRect = { closeBtnX + this->width / 2 - fontSurface->w / 2, closeBtnY + 5, fontSurface->w,fontSurface->h };
        }
        void draw(Mouse m) {
            // draw the background/window(what the colors will be on
            SDL_SetRenderDrawColor(this->renderer, 200, 200, 200, 200); // setting the color of the inner rect
            SDL_RenderFillRect(this->renderer, &(this->WindowRect)); // draw the filled in rect(inner rect)
            SDL_SetRenderDrawColor(this->renderer, 100, 100, 100, 150); // setting the color of the outer rect(for better looking)
            SDL_RenderDrawRect(this->renderer, &(this->WindowRect)); // draw the outline rect(outer rect)
            // draw the buttons
            this->closeScreenBtn.draw(m);
            // draw the text above("Drawing Modes Picker")
            SDL_RenderCopy(this->renderer, this->fontTexture, NULL, &(this->fontRect));

            // drawing the buttons
            drawCircle_Filled(this->renderer, this->drawingButtonsStartX, this->drawingButtonsStartY, this->circlesRadius, 0, 0, 0, 255);
            this->drawingButtonsStartX += this->circlesRadius * 2.5;
            drawCircle_SprayMode(this->renderer, this->drawingButtonsStartX, this->drawingButtonsStartY, this->circlesRadius+1, 0, 0, 0, 255); // adding 1 to the radius so it will look the same size
            this->drawingButtonsStartX += this->circlesRadius * 2.5;
            drawCircle_SprayMode2(this->renderer, this->drawingButtonsStartX, this->drawingButtonsStartY, this->circlesRadius + 1, 0, 0, 0, 255); // adding 1 to the radius so it will look the same size
            this->drawingButtonsStartX += this->circlesRadius * 2.5;
            drawCircle_Faded(this->renderer, this->drawingButtonsStartX, this->drawingButtonsStartY, this->circlesRadius + 1, 0, 0, 0, 255); // adding 1 to the radius so it will look the same size   

            // reseting the startX,startY variables
            this->drawingButtonsStartX = this->drawingButtonsStartXOriginal;
            this->drawingButtonsStartY = this->drawingButtonsStartYOriginal;

        }
        void handleMouse(Mouse m) {
            // filled circle
            if (checkCirclePointCollision(this->drawingButtonsStartX, this->drawingButtonsStartY, this->circlesRadius, m.x, m.y)) {
                cout << "Switched draw mode to: filled" << endl;
                this->drawMode = "filled";
            }
            this->drawingButtonsStartX += this->circlesRadius * 2.5;
            // sprayed circle
            if (checkCirclePointCollision(this->drawingButtonsStartX, this->drawingButtonsStartY, this->circlesRadius+1, m.x, m.y)) {
                cout << "Switched draw mode to: spray" << endl;
                this->drawMode = "spray";
            }
            this->drawingButtonsStartX += this->circlesRadius * 2.5;
            // sprayed2 circle
            if (checkCirclePointCollision(this->drawingButtonsStartX, this->drawingButtonsStartY, this->circlesRadius + 1, m.x, m.y)) {
                cout << "Switched draw mode to: spray2" << endl;
                this->drawMode = "spray2";
            }
            this->drawingButtonsStartX += this->circlesRadius * 2.5;
            // faded circle
            if (checkCirclePointCollision(this->drawingButtonsStartX, this->drawingButtonsStartY, this->circlesRadius + 1, m.x, m.y)) {
                cout << "Switched draw mode to: faded" << endl;
                this->drawMode = "faded";
            }

            // reseting the startX,startY variables
            this->drawingButtonsStartX = this->drawingButtonsStartXOriginal;
            this->drawingButtonsStartY = this->drawingButtonsStartYOriginal;
        }
        bool pressedToClosePicker(Mouse m) { // checking only when the user presses the left mouse button
            // return true when:
            // the user pressed the close button
            // the user pressed something outside the screen
            return this->closeScreenBtn.checkIfMouseOnBtn(m) || // check the button
                (m.x < this->closeBtnX || m.x > this->closeBtnX + this->width || m.y < this->closeBtnY || m.y > this->closeBtnY + this->height); // check if pressed on anything outside the window
        }
};
class CircleButton {
    public:
        SDL_Renderer* renderer;
        int x, y, radius;
        SDL_Color color;
        CircleButton(SDL_Renderer* renderer, int x, int y, int radius, SDL_Color color) {
            this->renderer = renderer;
            this->x = x;
            this->y = y;
            this->radius = radius;
            this->color = color;
        }
        CircleButton() { // empty constructor for when we want to create a variable of it and then set values
        }
        void draw() {
            drawCircle_Filled(this->renderer, this->x, this->y, this->radius, 255,255,255,255);
            drawCircle_NotFilled(this->renderer, this->x, this->y, this->radius, 0, 0, 0, 255);
            drawCircle_Filled(this->renderer, this->x, this->y, this->radius, this->color.r, this->color.g, this->color.b, this->color.a);
        }
        bool pressedButton(Mouse m) {
            return checkCirclePointCollision(this->x, this->y, this->radius, m.x, m.y);
        }
};
class ScreenCoverPicker {
    public:
        SDL_Renderer* renderer;
        SDL_Color currentCoverColor = scCreateSDL_ColorVariable(0, 0, 0, 0); // all of the covers alpha value will be between 50-100

        int closeBtnX, closeBtnY;
        int width = 400;
        int height = width;

        SDL_Rect FullWindowRect;
        SDL_Rect WindowRect;
        Button closeScreenBtn;

        TTF_Font* font;
        // for the headline of this mini window
        SDL_Surface* fontSurface;
        SDL_Texture* fontTexture;
        SDL_Rect fontRect;

        int circlesRadiuses = 20;
        vector<SDL_Color> buttonsColors = {
            // 9 colors each line
            // first line starts
            scCreateSDL_ColorVariable(0,0,0,0) // without cover
            ,scCreateSDL_ColorVariable(50,50,50,70),scCreateSDL_ColorVariable(205,192,176,100)
            ,scCreateSDL_ColorVariable(118,238,198,50),scCreateSDL_ColorVariable(255,105,180,50)
            ,scCreateSDL_ColorVariable(179,238,58,50),scCreateSDL_ColorVariable(99,184,255,50)
            ,scCreateSDL_ColorVariable(255,255,0,60),scCreateSDL_ColorVariable(139,139,0,80)
            // second line starts
            ,scCreateSDL_ColorVariable(139,34,82,100),scCreateSDL_ColorVariable(54,100,139,100)
            ,scCreateSDL_ColorVariable(139,131,120,100),scCreateSDL_ColorVariable(138,43,226,50)
            ,scCreateSDL_ColorVariable(156,102,31,60),scCreateSDL_ColorVariable(255,97,3,70)
            ,scCreateSDL_ColorVariable(61,89,171,100),scCreateSDL_ColorVariable(189,183,107,100)
            ,scCreateSDL_ColorVariable(47,79,79,60)
            // next two lines are the first 2 but with the alpha level +30
            // third line starts
            ,scCreateSDL_ColorVariable(0,0,0,0) // without cover
            ,scCreateSDL_ColorVariable(50,50,50,100),scCreateSDL_ColorVariable(205,192,176,130)
            ,scCreateSDL_ColorVariable(118,238,198,80),scCreateSDL_ColorVariable(255,105,180,80)
            ,scCreateSDL_ColorVariable(179,238,58,80),scCreateSDL_ColorVariable(99,184,255,80)
            ,scCreateSDL_ColorVariable(255,255,0,90),scCreateSDL_ColorVariable(139,139,0,110)
            // fourth line starts
            ,scCreateSDL_ColorVariable(139,34,82,130),scCreateSDL_ColorVariable(54,100,139,130)
            ,scCreateSDL_ColorVariable(139,131,120,130),scCreateSDL_ColorVariable(138,43,226,80)
            ,scCreateSDL_ColorVariable(156,102,31,90),scCreateSDL_ColorVariable(255,97,3,100)
            ,scCreateSDL_ColorVariable(61,89,171,130),scCreateSDL_ColorVariable(189,183,107,130)
            ,scCreateSDL_ColorVariable(47,79,79,90)
        };
        vector<CircleButton> buttons;
        ScreenCoverPicker(SDL_Renderer* renderer, int screenWidth1, int screenHeight1, TTF_Font* f, int startX, int startY) {
            this->renderer = renderer;
            this->closeBtnX = screenWidth1 / 2 - this->width / 2;
            this->closeBtnY = screenHeight1 / 2 - this->height / 2;
            this->FullWindowRect = srCreateRectObject(startX, startY,screenWidth1,screenHeight1);
            this->WindowRect = srCreateRectObject(this->closeBtnX, this->closeBtnY, this->width, this->height);
            this->closeScreenBtn = Button(this->closeBtnX, this->closeBtnY, scCreateSDL_ColorVariable(238, 59, 59, 255), "X", f, renderer);

            this->font = f;
            this->fontSurface = TTF_RenderText_Solid(f, "Cover picker", { 0,0,0,255 });
            this->fontTexture = SDL_CreateTextureFromSurface(renderer, fontSurface);
            this->fontRect = { closeBtnX + this->width / 2 - fontSurface->w / 2, closeBtnY + 5, fontSurface->w,fontSurface->h };

            // set the covers buttons
            int x = this->closeBtnX + this->circlesRadiuses + 10;
            int y = this->closeBtnY + this->WindowRect.h / 4;
            CircleButton cb;
            for (SDL_Color c : this->buttonsColors) {
                cb = CircleButton(this->renderer, x, y, this->circlesRadiuses, c);
                this->buttons.push_back(cb);
                x += this->circlesRadiuses*2 + 2;
                if (x + this->circlesRadiuses > this->WindowRect.x + this->WindowRect.w - 5) {
                    x = this->closeBtnX + this->circlesRadiuses + 10;
                    y += this->circlesRadiuses * 2 + 2;
                }
            }
        }
        void drawCover() {
            SDL_SetRenderDrawColor(this->renderer, this->currentCoverColor.r, this->currentCoverColor.g, this->currentCoverColor.b, this->currentCoverColor.a);
            SDL_RenderFillRect(this->renderer, &(this->FullWindowRect));
        }
        void draw(Mouse m) {
            // draw the background/window(what the colors will be on
            SDL_SetRenderDrawColor(this->renderer, 200, 200, 200, 200); // setting the color of the inner rect
            SDL_RenderFillRect(this->renderer, &(this->WindowRect)); // draw the filled in rect(inner rect)
            SDL_SetRenderDrawColor(this->renderer, 100, 100, 100, 150); // setting the color of the outer rect(for better looking)
            SDL_RenderDrawRect(this->renderer, &(this->WindowRect)); // draw the outline rect(outer rect)
            // draw the buttons
            this->closeScreenBtn.draw(m);
            // draw the text above("Drawing Modes Picker")
            SDL_RenderCopy(this->renderer, this->fontTexture, NULL, &(this->fontRect));

            // drawing the buttons
            for (CircleButton btn : this->buttons) {
                btn.draw();
            }
        }
        void handleMouse(Mouse m) {
            for (CircleButton btn : this->buttons) {
                if (btn.pressedButton(m) == true) {
                    this->currentCoverColor = btn.color;
                    break;
                }
            }
        }
        bool pressedToClosePicker(Mouse m) { // checking only when the user presses the left mouse button
            // return true when:
            // the user pressed the close button
            // the user pressed something outside the screen
            return this->closeScreenBtn.checkIfMouseOnBtn(m) || // check the button
                (m.x < this->closeBtnX || m.x > this->closeBtnX + this->width || m.y < this->closeBtnY || m.y > this->closeBtnY + this->height); // check if pressed on anything outside the window
        }
};

// classes for the shapes you can draw
class RectButton {
    public:
        SDL_Renderer* renderer;
        int x, y;
        int width, height;
        int type; // 0 filled in, 1 only outline
        SDL_Rect rect;
        RectButton(SDL_Renderer* r, int x, int y, int width, int height, int type) {
            this->renderer = r;
            this->x = x;
            this->y = y;
            this->width = width;
            this->height = height;
            this->type = type;
            this->rect = srCreateRectObject(x, y, width, height);
        }
        void draw() {
            if (this->type == 0) { // filled
                SDL_SetRenderDrawColor(this->renderer, 0, 0, 0, 100); // setting the color of the inner rect
                SDL_RenderFillRect(this->renderer, &(this->rect)); // draw the filled in rect(inner rect)
            }
            else { // only outline
                SDL_SetRenderDrawColor(this->renderer, 0, 0, 0, 255); // setting the color of the inner rect
                SDL_RenderDrawRect(this->renderer, &(this->rect));
            }
        }
        bool checkIfUserPressed(Mouse m) {
            if (this->x < m.x && this->x + this->width > m.x) {
                if (this->y < m.y && this->y + this->height > m.y) {
                    return true;
                }
            }
            return false;
        }
};
class PlaceWithMouseRect {
    public:
        SDL_Renderer* renderer;
        int type; // 0 filled in, 1 only outline
        int startX = -1;
        int startY = -1;
        int endX = -1;
        int endY = -1;
        SDL_Rect rect;
        PlaceWithMouseRect(SDL_Renderer* r) {
            this->renderer = r;
            this->startX = -1;
            this->startY = -1;
            this->endX = -1;
            this->endY = -1;
        }
        void draw(Mouse m) {
            // drawing faded white rect before the "real" one for when drawing rect on a black screen/other rect the user will know where he is drawing it
            if (startX != -1) {
                this->rect = srCreateRectObject(this->startX, this->startY, m.x - this->startX, m.y - this->startY);
                if (this->type == 0) { // filled
                    SDL_SetRenderDrawColor(this->renderer, 255,255,255, 50); // setting the color of the inner rect
                    SDL_RenderFillRect(this->renderer, &(this->rect)); // draw the filled in rect(inner rect)
                    SDL_SetRenderDrawColor(this->renderer, 0, 0, 0, 100); // setting the color of the inner rect
                    SDL_RenderFillRect(this->renderer, &(this->rect)); // draw the filled in rect(inner rect)
                }
                else { // only outline
                    SDL_SetRenderDrawColor(this->renderer, 255, 255, 255, 50); // setting the color of the inner rect
                    SDL_RenderDrawRect(this->renderer, &(this->rect));
                    SDL_SetRenderDrawColor(this->renderer, 0, 0, 0, 255); // setting the color of the inner rect
                    SDL_RenderDrawRect(this->renderer, &(this->rect));
                }
            }
        }
};
class RectToDraw {
    public:
        SDL_Renderer* renderer;
        int type; // 0 filled in, 1 only outline
        int startX, startY;
        int endX, endY;
        SDL_Rect rect;
        SDL_Color color;

        RectToDraw(SDL_Renderer* r, int type, int startX, int startY, int endX, int endY, SDL_Color c) {
            this->renderer = r;
            this->type = type;
            this->startX = startX;
            this->startY = startY;
            this->endX = endX;
            this->endY = endY;
            this->rect = srCreateRectObject(startX, startY, endX - startX, endY - startY);
            this->color = c;
        }

        void drawFinishedRect() {
            if (this->type == 0) { // filled
                SDL_SetRenderDrawColor(this->renderer, this->color.r, this->color.g, this->color.b, this->color.a); // setting the color of the inner rect
                SDL_RenderFillRect(this->renderer, &(this->rect)); // draw the filled in rect(inner rect)
            }
            else { // only outline
                SDL_SetRenderDrawColor(this->renderer, this->color.r, this->color.g, this->color.b, this->color.a); // setting the color of the inner rect
                SDL_RenderDrawRect(this->renderer, &(this->rect));
            }
        }
};

class LineButton {
    public:
        SDL_Renderer* renderer;
        int x, y;
        int length;
        int spaceToCheckPress;
        SDL_Rect btnRect;
        LineButton(SDL_Renderer* r, int x, int y, int length, int spaceToCheckPress) {
            this->renderer = r;
            this->x = x;
            this->y = y;
            this->length = length;
            this->spaceToCheckPress = spaceToCheckPress;
            this->btnRect = srCreateRectObject(x-4, y - spaceToCheckPress, length+8, spaceToCheckPress * 2);
        }
        void draw() {
            SDL_SetRenderDrawColor(this->renderer, 152, 245, 255, 100);
            SDL_RenderFillRect(this->renderer, &(this->btnRect));

            SDL_SetRenderDrawColor(this->renderer, 0,0,0, 255);
            SDL_RenderDrawLine(this->renderer, this->x, this->y, this->x + this->length, this->y);

            drawCircle_NotFilled(this->renderer, this->x, this->y, 3, 0, 0, 0, 255);
            drawCircle_NotFilled(this->renderer, this->x + this->length, this->y, 3, 0, 0, 0, 255);
        }
        bool checkIfUserPressed(Mouse m) {
            if (m.x > this->x - 4 && m.x < this->x - 4 + this->length + 8) {
                if (m.y > this->y - this->spaceToCheckPress && m.y < this->y + this->spaceToCheckPress) {
                    return true;
                }
            }
            return false;
        }
};
class PlaceWithMouseLine {
    public:
        SDL_Renderer* renderer;
        int x1 = -1;
        int y1 = -1;
        int x2 = -1;
        int y2 = -1;
        PlaceWithMouseLine(SDL_Renderer* r) {
            this->renderer = r;
            this->x1 = -1;
            this->y1 = -1;
            this->x2 = -1;
            this->y2 = -1;
        }
        void draw(Mouse m) {
            if (this->x1 != -1) {
                SDL_SetRenderDrawColor(this->renderer, 0, 0, 0, 255);
                SDL_RenderDrawLine(this->renderer, this->x1, this->y1, m.x, m.y);
                // same reason we do it in rect
                // the faded line for when drawing on a black rect/screen
                SDL_SetRenderDrawColor(this->renderer, 255,255,255, 100);
                SDL_RenderDrawLine(this->renderer, this->x1, this->y1, m.x, m.y);
            }
        }
};
class LineToDraw {
    public:
        SDL_Renderer* renderer;
        int x1, y1, x2, y2;
        SDL_Color color;
        LineToDraw(SDL_Renderer* r, int x1, int y1, int x2, int y2, SDL_Color c) {
            this->renderer = r;
            this->color = c;
            this->x1 = x1;
            this->y1 = y1;
            this->x2 = x2;
            this->y2 = y2;
        }
        void draw() {
            SDL_SetRenderDrawColor(this->renderer, this->color.r, this->color.g, this->color.b, this->color.a);
            SDL_RenderDrawLine(this->renderer, this->x1, this->y1, this->x2, this->y2);
        }

};

class PointToDraw {
    public:
        int x, y;
        int radius;
        SDL_Color color;
        string drawMode;
        PointToDraw(int x, int y, int radius, SDL_Color color, string drawMode) {
            this->x = x;
            this->y = y;
            this->radius = radius;
            this->color = color;
            this->drawMode = drawMode;
        }
        PointToDraw() {
            // empty constructor for when we want to create a point variable but after change the whole variable
        }
        void drawPoint(SDL_Renderer* r) {
            if (this->drawMode == "filled") {
                drawCircle_Filled(r, this->x, this->y, this->radius, this->color.r, this->color.g, this->color.b, this->color.a);
            }
            else if (this->drawMode == "spray") {
                drawCircle_SprayMode(r, this->x, this->y, this->radius, this->color.r, this->color.g, this->color.b, this->color.a);
            }
            else if (this->drawMode == "spray2") {
                drawCircle_SprayMode2(r, this->x, this->y, this->radius, this->color.r, this->color.g, this->color.b, this->color.a);
            }
            else if (this->drawMode == "faded") {
                drawCircle_Faded(r, this->x, this->y, this->radius, this->color.r, this->color.g, this->color.b, this->color.a);
            }
        }
};
class DrawingHandler {
    public:
        SDL_Renderer* renderer;
        vector<PointToDraw> points; // the points we are going to draw
        vector<RectToDraw> rects;
        vector<LineToDraw> lines;
        vector<int> drawTurns; // 1-point, 2-rect, 3-line  (this variable is for being able to know when to draw what, for if a point is on a rect it will show like this too)
        int drawX = 135;
        int drawY = 35;
        int width, height;
        SDL_Rect drawScreenRect;

        string imagePath = "";
        SDL_Surface* image;// = IMG_Load("image.png");
        SDL_Texture* imageTexture;
        SDL_Rect imageRect;

        DrawingHandler(SDL_Renderer* r,int screenWidth, int screenHeight) {
            this->renderer = r;
            this->width = screenWidth - this->drawX - 1;
            this->height = screenHeight - this->drawY - 1;
            this->drawScreenRect = srCreateRectObject(this->drawX, this->drawY, this->width, this->height);
        }
        void resetAll() {
            // problem:
            // when one of the vectors has something in it we get an error
            this->points = {};
            this->rects = {};
            this->lines = {};
            this->drawTurns = {};
        }

        void addImage(string path) {
            if (path == "") { // if the path is blank it will quit the function
                return;
            }
            try {
                this->imagePath = path;
                string str(path);
                this->image = IMG_Load(str.c_str());
                this->imageTexture = SDL_CreateTextureFromSurface(this->renderer, this->image);
                SDL_FreeSurface(this->image);
                this->imageRect = srCreateRectObject(this->drawX, this->drawY, this->width, this->height);
            }
            catch (exception& e){
                e.what(); // prints info about the error
            }
        }
        void removeImage() {
            // the drawing is based on the path so when we do this we tell the program not to display the image
            this->imagePath = "";
        }
        void addPoint(int x, int y, int radius, SDL_Color c, string drawMode) {
            if (y > this->drawY + radius && y + radius < this->drawY + this->height
                && x > this->drawX + radius && x + radius < this->drawX + this->width) { //  if its in the draw area
                bool add = true;
                for (PointToDraw p : this->points) {
                    if (p.radius = radius && p.x == x && p.y == y  && p.color.r == c.r && p.color.g == c.g && p.color.b == c.b) {
                        add = false;
                    }
                }
                if (add == true && c.a == 0) { // if the alpha level is 0 we won't be able to see the point anyway so we wont place it
                    add = false;
                }
                if (add == true) {
                    this->points.push_back(
                        PointToDraw(x, y, radius, c, drawMode)
                    );
                    this->drawTurns.push_back(1);
                }
            }
        }
        void addRect(RectToDraw r) {
            this->rects.push_back(r);
            this->drawTurns.push_back(2);
        }
        void addLine(LineToDraw l) {
            this->lines.push_back(l);
            this->drawTurns.push_back(3);
        }
        
        bool collideWithSomething(int x, int y, int radius) {
            cout << "turns:" << this->drawTurns.size() << " points:" << this->points.size() << " rects:" << this->rects.size() << " lines:" << this->lines.size() << endl;
            // check points collision
            // using check circles collision
            for (PointToDraw p : this->points) {
                // if collide with a point that is not white
                if ( !(p.color.r == 255 && p.color.g == 255 && p.color.b == 255) && checkCirclesCollision(p.x, p.y, p.radius, x, y, radius) == true) {
                    return true;
                }
            }
            // check rects collision
            // using rects collision(not so precise but good enough for this
            SDL_Rect circleAsRect = srCreateRectObject(x, y, radius * 2, radius * 2);
            SDL_Rect placedRectRect;
            for (RectToDraw r : this->rects) {
                placedRectRect = srCreateRectObject(r.startX, r.startY, r.endX - r.startX, r.endY - r.startY);
                if (checkRectsCollision(circleAsRect, placedRectRect) == true) {
                    return true;
                }
            }
            // check lines collision
            // getting the closest point to the center of the circle on the line and checking if its smaller/equal to the circle radius
            int lineX1, lineY1, lineX2, lineY2;
            for (LineToDraw l : this->lines) {
                lineX1 = l.x1;
                lineY1 = l.y1;
                lineX2 = l.x2;
                lineY2 = l.y2;
                if (checkCircleLineCollision(x, y, radius, lineX1, lineY1, lineX2, lineY2) == true) {
                    return true;
                }
            }
            return false;
        }
        void erase(int x, int y, int radius) {
            if (this->collideWithSomething(x, y, radius) == true || this->imagePath != "") {
                PointToDraw p = PointToDraw(1, 2, 3, scCreateSDL_ColorVariable(2, 2, 2, 255), "filled");
                this->addPoint(x, y, radius, scCreateSDL_ColorVariable(255, 255, 255, 255), "filled");
            }
        }
        
        void drawAllPoints() {
            for (PointToDraw point : this->points) {
                point.drawPoint(this->renderer);
            }
        }
        void drawSpecificPoint(int index) {
            this->points.at(index).drawPoint(this->renderer);
        }
        void drawAllRects() {
            for (RectToDraw r : this->rects) {
                r.drawFinishedRect();
            }
        }
        void drawSpecifcRect(int index) {
            this->rects.at(index).drawFinishedRect();
        }
        void drawAllLines() {
            for (LineToDraw l : this->lines) {
                l.draw();
            }
        }
        void drawSpecificLine(int index) {
            this->lines.at(index).draw();
        }
        void drawAll(SDL_Surface* s) {
            // draws the image
            if (this->imagePath != "") {
                SDL_RenderCopy(this->renderer, this->imageTexture, NULL, &(this->imageRect));
            }

            // draw the drawing area outline
            SDL_SetRenderDrawColor(this->renderer, 0, 0, 0, 255);
            SDL_RenderDrawRect(this->renderer, &(this->drawScreenRect));
            // draw the points+rects
            int pointsIndex = 0;
            int rectsIndex = 0;
            int linesIndex = 0;
            //this->drawAllPoints();
            //this->drawAllRects();
            //this->drawAllLines();
            //cout << "turns:" << this->drawTurns.size() << " points:" << this->points.size() << " rects:" << this->rects.size() << " lines:" << this->lines.size() << endl;
            for (int turn : this->drawTurns) {
                if (turn == 1) { // point
                    this->drawSpecificPoint(pointsIndex);
                    pointsIndex += 1;
                }
                else if (turn == 2) { // rect
                    this->drawSpecifcRect(rectsIndex);
                    rectsIndex += 1;
                }
                else if (turn == 3) {
                    this->drawSpecificLine(linesIndex);
                    linesIndex += 1;
                }
            }

        }
};

// radius changer buttons
class RadiusButtons {
    public:
        SDL_Renderer* renderer;
        int textX, textY;
        int radius3X, radius3Y;
        int radius5X, radius5Y;
        int radius8X, radius8Y;
        SDL_Color textColor = { 0,0,0 ,255 }; // color of the text
        TTF_Font* font;
        SDL_Surface* textSurface;
        SDL_Texture* textTexture;
        SDL_Rect textRect;
        SDL_Rect rect3, rect5, rect8;
        RadiusButtons(int x, int y, SDL_Renderer* r, TTF_Font* f) {
            this->renderer = r;
            this->font = f;
            this->textX = x;
            this->textY = y;
            this->textSurface = TTF_RenderText_Solid(this->font, "Radiuses:", this->textColor);
            this->textTexture = SDL_CreateTextureFromSurface(r, this->textSurface);
            this->textRect = { this->textX, this->textY, this->textSurface->w,this->textSurface->h };

            this->radius3Y = y + textSurface->h / 2;
            this->radius5Y = y + textSurface->h / 2;
            this->radius8Y = y + textSurface->h / 2;

            this->radius3X = x + 5 + textSurface->w + 3 + 4;
            this->radius5X = this->radius3X + 3 + 5 + 4;
            this->radius8X = this->radius5X + 5 + 8 + 4;

            this->rect3 = srCreateRectObject(this->radius3X - 3, 0, 6, this->textSurface->h);
            this->rect5 = srCreateRectObject(this->radius5X - 5, 0, 10, this->textSurface->h);
            this->rect8 = srCreateRectObject(this->radius8X - 8, 0, 16, this->textSurface->h);
        }
        void draw() {
            SDL_RenderCopy(this->renderer, this->textTexture, NULL, &(this->textRect));
            SDL_SetRenderDrawColor(this->renderer, 152, 245, 255,255);

            SDL_RenderFillRect(this->renderer, &(this->rect3)); // draw the filled in rect(inner rect)
            SDL_RenderFillRect(this->renderer, &(this->rect5)); // draw the filled in rect(inner rect)
            SDL_RenderFillRect(this->renderer, &(this->rect8)); // draw the filled in rect(inner rect)

            drawCircle_NotFilled(this->renderer, this->radius3X, this->radius3Y, 3, this->textColor.r, this->textColor.g, this->textColor.b, this->textColor.a);
            drawCircle_NotFilled(this->renderer, this->radius5X, this->radius5Y, 5, this->textColor.r, this->textColor.g, this->textColor.b, this->textColor.a);
            drawCircle_NotFilled(this->renderer, this->radius8X, this->radius8Y, 8, this->textColor.r, this->textColor.g, this->textColor.b, this->textColor.a);
        }
        int handleMouse(Mouse m, MouseCircle mc, DrawingHandler dh) {
            if (m.y < dh.drawY) { // if its in the buttons area(will give us the option to check only the x position)
                if (m.x > this->radius3X - 3 && m.x < this->radius8X + 8) { // in the radiuses buttons area
                    if (m.x > this->radius3X - 3 && m.x < this->radius3X + 3) {
                        return 3;
                    } else if (m.x > this->radius5X - 5 && m.x < this->radius5X + 5) {
                        return 5;
                    } else if (m.x > this->radius8X - 8 && m.x < this->radius8X + 8) {
                        return 8;
                    }
                }
            }
            return mc.radius;
        }
};

int main(int argc, char* argv[]) {
    // the GetSystemMetrics(SM_CXSCREEN) and GetSystemMetrics(SM_CYSCREEN) gets the screen width and height
    static int screenWidth = GetSystemMetrics(SM_CXSCREEN);
    static int screenHeight = GetSystemMetrics(SM_CYSCREEN) - 80; // the -80 is for the user being able to press on the X button that is built in

    TTF_Init();
    IMG_Init(IMG_INIT_PNG | IMG_INIT_JPG);
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        cout << "Failed to initialize the SDL2 library\n";
        return -1;
    }

    SDL_Window* window = SDL_CreateWindow("Drawing engine",
        0,0+30,
        screenWidth, screenHeight,
        0);
    if (!window){
        cout << "Failed to create window\n";
        return -1;
    }
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, 0); // creating a render so we will be able to do stuff on the screen;
    if (!renderer) {
        cout << "Failed to create renderer\n";
        return -1;
    }

    TTF_Font* font20 = TTF_OpenFont("fonts\\MADEINFINITY PERSONALUSE-Regular.otf", 20);
    TTF_Font* font15 = TTF_OpenFont("fonts\\MADEINFINITY PERSONALUSE-Regular.otf", 15);
    TTF_Font* font15Different = TTF_OpenFont("fonts\\Cocogoose Pro-trial.ttf", 15);

    // creating the variables used for the classes
    vector<string> TextForButtons = { "Save Draw","New Draw","Color Picker","Covers","Erase","Open/Remove Image"}; // for cButtons
    vector<SDL_Color> ColorsForButtons = { scCreateSDL_ColorVariable(202,255,112,255),scCreateSDL_ColorVariable(193,205,205,255)
    ,scCreateSDL_ColorVariable(142,229,238,255),scCreateSDL_ColorVariable(162,205,200,255), scCreateSDL_ColorVariable(224,238,238,150),
    scCreateSDL_ColorVariable(102,205,170,255)
    }; // for cButtons
    vector<TTF_Font*> fontsForButtons = { font20 ,font20,font20,font20,font20,font20};
    vector<SDL_Renderer*> renderersForButtons = {renderer,renderer,renderer,renderer,renderer,renderer};
    // creating the variables of the classes i made
    Buttons cButtons = Buttons(renderersForButtons,TextForButtons, ColorsForButtons,fontsForButtons, 20,1); // getting: text on button, button color, startX, startY
    int lastBtnX = cButtons.buttons.at(cButtons.buttons.size()-1).x + cButtons.buttons.at(cButtons.buttons.size() - 1).width + 2;
    RadiusButtons cRadiusesButtons = RadiusButtons(lastBtnX,1,renderer, font20);
    lastBtnX = cRadiusesButtons.radius8X + 8 + 2; // +8 to reach the end of the button, +2 for it to look good
    cButtons.addButton("Drawing Modes", scCreateSDL_ColorVariable(162, 227, 255,255), lastBtnX, 1, font20, renderer);

    Mouse cMouse = Mouse();
    MouseCircle cMouseCircle = MouseCircle();

    // shapes to draw stuff(buttons...)
    int shapesButtonsY = 70;
    RectButton cFilledRectBtn = RectButton(renderer, 10, shapesButtonsY, 70, 70, 0);
    shapesButtonsY += 70 + 10;
    RectButton cNotFilledRectBtn = RectButton(renderer, 10, shapesButtonsY, 70, 70, 1);
    PlaceWithMouseRect cPLaceWithMouseRect = PlaceWithMouseRect(renderer);
    shapesButtonsY += 70 + 20;
    LineButton cLineButton = LineButton(renderer, 10, shapesButtonsY, 70, 10);
    PlaceWithMouseLine cPlaceWithMouseLine = PlaceWithMouseLine(renderer);

    DrawingHandler cDrawingHandler = DrawingHandler(renderer, screenWidth, screenHeight);
    ColorPicker cColorPicker = ColorPicker(renderer,0,0,0,screenWidth,screenHeight,font20);
    DrawModePicker cDrawModePicker = DrawModePicker(renderer,screenWidth,screenHeight,font20);
    ScreenCoverPicker cScreenCoverPicker = ScreenCoverPicker(renderer, screenWidth, screenHeight, font20, cDrawingHandler.drawX, cDrawingHandler.drawY);

    SDL_Surface* window_surface = SDL_GetWindowSurface(window);

    if (!window_surface)
    {
        cout << "Failed to get the surface from the window\n";
        return -1;
    }

    SDL_UpdateWindowSurface(window);
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND); // changing the color mode so when you do SDL_SetRenderDrawColor() with an alpha level it will change the transparency

    bool bGameOn = true;
    string baseMode = "drawing";
    string placingFilledRectMode = "filled rect placing";
    string placingNotFilledRectMode = "not filled rect placing";
    string placingLineMode = "line placing";
    string currentMode = baseMode;

    int fps = 0;
    time_t startTime = time(0);
    SDL_Event e;
    while (bGameOn) {
        //cout << currentMode << endl;
        fps += 1;
        if (difftime(time(0), startTime) >= 1.0) {
            cout << "fps: " << fps << endl;
            startTime = time(0);
            fps = 0;
        }
        // event checker
        while (SDL_PollEvent(&e) > 0) {
            if (e.type == SDL_QUIT) {
                bGameOn = false;
                break;
            } else if (e.type == SDL_MOUSEMOTION){
                cMouse.vUpdateMousePos();
            }
            if (e.type == SDL_MOUSEBUTTONDOWN) {
                if (e.button.button == SDL_BUTTON_LEFT) {
                    cMouseCircle.leftMouseButtonPressed = true;// to change the circle animation
                    if (currentMode == baseMode) { // drawing
                        cMouseCircle.radius = cRadiusesButtons.handleMouse(cMouse, cMouseCircle, cDrawingHandler);
                        if (cFilledRectBtn.checkIfUserPressed(cMouse) == true) {
                            currentMode = placingFilledRectMode;
                            cPLaceWithMouseRect.type = 0;
                        }
                        else if (cNotFilledRectBtn.checkIfUserPressed(cMouse) == true) {
                            currentMode = placingNotFilledRectMode;
                            cPLaceWithMouseRect.type = 1;
                        }
                        else if (cLineButton.checkIfUserPressed(cMouse) == true) {
                            currentMode = placingLineMode;
                        }
                        else {
                            for (Button btn : cButtons.buttons) {
                                if (btn.checkIfMouseOnBtn(cMouse) == true) {
                                    cout << btn.text << " btn pressed" << endl;
                                    if (btn.text == TextForButtons.at(0)) { // save draw
                                        // save the current draw
                                        string fullFilePath = openFileDialogToSaveFile(); // opens a dialog for the user to choose where to save the draw
                                        takeAScreenShot(window_surface, renderer,fullFilePath,
                                            cDrawingHandler.drawX, cDrawingHandler.drawY, cDrawingHandler.width, cDrawingHandler.height);
                                    }
                                    else if (btn.text == TextForButtons.at(1)) { // new draw
                                        cDrawingHandler.resetAll();
                                    }
                                    else if (btn.text == TextForButtons.at(2)) { // color picker
                                        currentMode = btn.text;
                                    }
                                    else if (btn.text == TextForButtons.at(3)) { // covers button(add a cover to the screen)
                                        currentMode = btn.text;
                                    }
                                    else if (btn.text == TextForButtons.at(4)) { // erase
                                         currentMode = btn.text;
                                    }
                                    else if (btn.text == TextForButtons.at(5)) { // open/remove image
                                        if (cDrawingHandler.imagePath == "") {
                                            string userImagePath = openFileDialogToOpenFile(); // opens a dialog for the user to choose what file to display
                                            // opens the file explorer and lets him choose an image+make it show
                                            cDrawingHandler.addImage(userImagePath);
                                        } else {
                                            cDrawingHandler.removeImage();
                                        }
                                    }
                                    else if (btn.text == "Drawing Modes") { // drawing modes
                                        currentMode = "Drawing Modes";
                                    }
                                    break; // breaking the for loop because we found on what he pressed
                                }
                            }
                        }
                    }
                    /* in the next few ifs(only the ones where the currentMode variable changes)
                     doing this line cMouseCircle.leftMouseButtonPressed = false;
                     for when the user do MOUSEBUTTONUP it won't draw a point at the position he is on now
                     basicaly for doing clean shapes without point draw at the finish x,y*/
                    else if (currentMode == TextForButtons.at(2)) { // color picker
                        if (cColorPicker.pressedToClosePicker(cMouse) == true) {
                            currentMode = baseMode; // return to drawing mode
                            cMouseCircle.leftMouseButtonPressed = false; // so it wont start drawing points
                        }
                    }
                    else if (currentMode == "Drawing Modes") { // color picker
                        if (cDrawModePicker.pressedToClosePicker(cMouse) == true) {
                            currentMode = baseMode; // return to drawing mode
                            cMouseCircle.leftMouseButtonPressed = false; // so it wont start drawing points
                        }
                    }
                    else if (currentMode == TextForButtons.at(3)) { // covers btn
                        if (cScreenCoverPicker.pressedToClosePicker(cMouse) == true) {
                            currentMode = baseMode; // return to drawing mode
                            cMouseCircle.leftMouseButtonPressed = false; // so it wont start drawing points
                        }
                    }
                    else if (currentMode == TextForButtons.at(4)) { // erase btn
                        for (Button btn : cButtons.buttons) {
                            if (btn.checkIfMouseOnBtn(cMouse) == true) {
                                if (currentMode == btn.text) {
                                    currentMode = baseMode;
                                }
                            }
                        }
                    }
                    else if (currentMode == placingFilledRectMode || currentMode == placingNotFilledRectMode) { // placing rect
                        if (cPLaceWithMouseRect.startX == -1) {
                            bool goodToDraw = true;
                            if (cMouse.x < cDrawingHandler.drawX) {
                                goodToDraw = false;
                            }
                            else if (cMouse.x > cDrawingHandler.drawX + cDrawingHandler.width) {
                                goodToDraw = false;
                            }
                            if (cMouse.y < cDrawingHandler.drawY) {
                                goodToDraw = false;
                            }
                            else if (cMouse.y > cDrawingHandler.drawY + cDrawingHandler.height) {
                                goodToDraw = false;
                            }
                            if (goodToDraw == true) {
                                cPLaceWithMouseRect.startX = cMouse.x;
                                cPLaceWithMouseRect.startY = cMouse.y;
                            }
                        }
                        else {
                            int endX = cMouse.x;
                            int endY = cMouse.y;
                            if (endX < cDrawingHandler.drawX) {
                                endX = cDrawingHandler.drawX;
                            }else if (endX > cDrawingHandler.drawX + cDrawingHandler.width) {
                                endX = cDrawingHandler.drawX + cDrawingHandler.width;
                            }
                            if (endY < cDrawingHandler.drawY) {
                                endY = cDrawingHandler.drawY;
                            }
                            else if (endY > cDrawingHandler.drawY + cDrawingHandler.height) {
                                endY = cDrawingHandler.drawY + cDrawingHandler.height;
                            }
                            RectToDraw r = RectToDraw(renderer, cPLaceWithMouseRect.type, cPLaceWithMouseRect.startX, cPLaceWithMouseRect.startY, endX,endY,
                                scCreateSDL_ColorVariable(cColorPicker.r, cColorPicker.g, cColorPicker.b, cColorPicker.a));
                            cDrawingHandler.addRect(r);
                            cPLaceWithMouseRect = PlaceWithMouseRect(renderer); // reset the class
                            currentMode = baseMode;
                            cMouseCircle.leftMouseButtonPressed = false; // so it wont start drawing points
                        }
                    }
                    else if (currentMode == placingLineMode) {
                        bool goodToDraw = true;
                        if (cMouse.x < cDrawingHandler.drawX) {
                            goodToDraw = false;
                        }
                        else if (cMouse.x > cDrawingHandler.drawX + cDrawingHandler.width) {
                            goodToDraw = false;
                        }
                        if (cMouse.y < cDrawingHandler.drawY) {
                            goodToDraw = false;
                        }
                        else if (cMouse.y > cDrawingHandler.drawY + cDrawingHandler.height) {
                            goodToDraw = false;
                        }
                        
                        if (cPlaceWithMouseLine.x1 == -1) {
                            if (goodToDraw == true) {
                                cPlaceWithMouseLine.x1 = cMouse.x;
                                cPlaceWithMouseLine.y1 = cMouse.y;
                            }
                        }
                        else {
                            int endX = cMouse.x;
                            int endY = cMouse.y;
                            if (goodToDraw == true) {
                                LineToDraw line = LineToDraw(renderer, cPlaceWithMouseLine.x1, cPlaceWithMouseLine.y1, endX, endY,
                                    scCreateSDL_ColorVariable(cColorPicker.r, cColorPicker.g, cColorPicker.b, cColorPicker.a));
                                cDrawingHandler.addLine(line);
                                cPlaceWithMouseLine = PlaceWithMouseLine(renderer); //reset the variable
                                currentMode = baseMode;
                                cMouseCircle.leftMouseButtonPressed = false; // so it wont start drawing points
                            }
                        }
                    }
                    
                }
            }
            else if (e.type == SDL_MOUSEBUTTONUP) {
                cMouseCircle.leftMouseButtonPressed = false; // to change the circle animation
            }
        }

        // updating the variable cMouse.showMouse if needed
        if (cMouse.showMouse == true && currentMode == "drawing") { // if drawing no need for the mouse to show
            cMouse.showMouse = false;
        } else if (cMouse.showMouse = true && currentMode == TextForButtons.at(4)){ // if in erase mode and the mouse is showing
            cMouse.showMouse = false;
        }else if (cMouse.showMouse == false && currentMode != "drawing" && currentMode != TextForButtons.at(4)) { // if not in drawing and the mouse isnt showing(not in erase mode)
            cMouse.showMouse = true;
        }
        if (cMouse.x < cDrawingHandler.drawX || cMouse.y < cDrawingHandler.drawY) { // if the user mouse is off the drawing area
            cMouse.showMouse = true;
        }
        
        // filling the background white
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255); // parameters; render, red value,green value, blue value, alpha level
        SDL_RenderClear(renderer);// clrear the current rendering target
        
        // drawing what the user want
        cDrawingHandler.drawAll(window_surface);

        // drawing the buttons
        for (Button b : cButtons.buttons) {
            b.draw(cMouse);
        }
        cRadiusesButtons.draw();

        // draw the rect buttons
        blitText(10, 50, scCreateSDL_ColorVariable(0, 0, 0, 255), "Shapes to use", font15Different, renderer);
        cFilledRectBtn.draw();
        cNotFilledRectBtn.draw();
        cLineButton.draw();
        
        if (currentMode == placingFilledRectMode || currentMode == placingNotFilledRectMode) {
            cPLaceWithMouseRect.draw(cMouse);
        }
        else if (currentMode == placingLineMode) {
            cPlaceWithMouseLine.draw(cMouse);
        }

        // if didnt place a shape now it will check if pressed on anything
        if (cMouseCircle.leftMouseButtonPressed == true) { // a way of checking when the mouse left button is being pressed
            if (currentMode == baseMode) {
                cDrawingHandler.addPoint(
                    cMouse.x,cMouse.y,cMouseCircle.radius,scCreateSDL_ColorVariable(cColorPicker.r, cColorPicker.g, cColorPicker.b, cColorPicker.a), cDrawModePicker.drawMode
                );
            } else if (currentMode == TextForButtons.at(2)) { // color picker
                cColorPicker.handleMouse(cMouse);
            }
            else if (currentMode == "Drawing Modes") { // draw mode picker
                cDrawModePicker.handleMouse(cMouse);
            }
            else if (currentMode == TextForButtons.at(3)) { // covers picker
                cScreenCoverPicker.handleMouse(cMouse);
            }
            else if (currentMode == TextForButtons.at(4)) { // color picker
                cDrawingHandler.erase(cMouse.x, cMouse.y, cMouseCircle.radius);
            }
        }

        // the cover needs to be above everything that is not a screen so we draw it last
        cScreenCoverPicker.drawCover();

        // color picker
        if (currentMode == TextForButtons.at(2)) {
            cColorPicker.draw(cMouse);
        }
        else if (currentMode == "Drawing Modes") {
            cDrawModePicker.draw(cMouse);
        }
        else if (currentMode == TextForButtons.at(3)) {
            cScreenCoverPicker.draw(cMouse);
        }

        // updating mouse pos and checking if to make it visible
        cMouse.vUpdateMousePos();
        if (cMouse.showMouse == true) { // when trying to press a button 
            SDL_ShowCursor(SDL_ENABLE); // show mouse pointer
        } else { // when drawing the mouse pointer won't be on the screen
            SDL_ShowCursor(SDL_DISABLE); // hide mouse pointer
            cMouseCircle.draw(cMouse, renderer, cColorPicker.r, cColorPicker.g, cColorPicker.b, cColorPicker.a);
        }

        // showing what we did in the renderer
        SDL_RenderPresent(renderer); // updates the screen with the current changes of the render variable
    }

    TTF_Quit();
    IMG_Quit();
    SDL_Quit();
    return 0;
}