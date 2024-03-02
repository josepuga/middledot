#include <cstddef>
#include <cstdint>

// Funcion necesaria ya que no podemos usar <cstring>
size_t strlen(const char* str);


// Constantes para manejar la VGA
namespace vga {

enum color : uint8_t { // https://www.fountainware.com/EXPL/vga_color_palettes.htm
    black = 0,
    blue = 1,
    green = 2,
    cyan = 3,
    red = 4,
    magenta = 5,
    brown = 6,
    light_grey = 7,
    dark_grey = 8,
    light_blue = 9,
    light_green = 10,
    light_cyan = 11,
    light_red = 12,
    light_magenta = 13,
    yellow = 14, //light_brown?
    white = 15,
};

// Límites
constexpr uint8_t max_rows = 25;
constexpr uint8_t max_cols = 80;
constexpr size_t buffer_size = max_rows * max_cols;

// Esta es la dirección de memoria inicial con un buffer 16bits de 80*24 de 
// tamaño. Cualquier cosa que se escriba aquí, la BIOS se encarga de mostrarlo
// en pantalla.
constexpr size_t memory_start = 0xB8000;

} // namespace

// Manejamos la VGA mediante una clase, mediante funciones rudimentarias,
// podremos escribir texto en la pantalla
class Vga {
    private:
        uint8_t fg_color {vga::color::light_grey};
        uint8_t bg_color {vga::color::black};
        uint8_t vga_color {0};
        uint16_t *video_buffer {(uint16_t*) vga::memory_start};
        size_t index {0}; //Buffer index

        void putchar(const char);

    public:
        // Posiciona el cursor en la columna x, fila y.
        void gotoxy(const size_t x, const size_t y);

        // Asigna los colores de tinta y fondo.
        void setcolors(const uint8_t fg, const uint8_t bg);

        // Imprime una cadena en la posición actual con el color actual
        void print(const char*);

        // Borra la pantalla con los colores actuales.
        void clear();
};

void Vga::setcolors(const uint8_t fg, const uint8_t bg) {
    fg_color = fg;
    bg_color = bg;
    // Con aritmética binaria se encajan ambos en 8 bits.
    // El primer nibble tendrá el fondo, y el segundo la tinta.
    vga_color = fg | bg << 4;  
}

void Vga::gotoxy(const size_t x, const size_t y) {
    // Overflow check.
    size_t _x = x >= vga::max_cols ? vga::max_cols-1 : x;
    size_t _y = y >= vga::max_rows ? vga::max_rows-1 : y;

    index = vga::max_cols * _y + _x;// _y * vga::max_cols * _x;
}

void Vga::putchar(const char c) {
    // La entrada es de 16 bits, 8 para los colores y 8 para el carácter.
    uint16_t entry = c;
    video_buffer[index] = entry | vga_color << 8;

}

void Vga::clear() {
    for (index = 0; index < vga::buffer_size; index++) {
        putchar(' ');
    }
    index = 0;
}

void Vga::print(const char* msg) {
    for (size_t n = 0; msg[n] != 0; n++, index++) {
        // Comprobar que index no pase el buffer de la VGA. Se deja de imprimir
        // Gestionar esto requiere que se haga un código que admita "scroll" 
        // y guarde las lineas en un buffer.
        if (index >= vga::buffer_size) {
            break;
        }
        //TODO: Gestionar saltos de linea y llevar control de fila/columna.
        putchar(msg[n]);
    }
}


size_t strlen(const char* str) {
    size_t result {0};
    while (str[result] != 0) {
        result++;
    }
    return result;
}

// En lugar de main, usamos esta función. Hay que user extern "C" para evitar
// el mangling.
extern "C" {
void kmain() {  
    Vga v;
    v.clear();
    v.setcolors(vga::color::white, vga::color::black);

    v.gotoxy(4,1);
    v.print("MiddleDot - v0.0.1alpha");

    // Una linea con los colores disponibles
    v.gotoxy(42, 22);
    for (uint8_t n = vga::color::black; n <= vga::color::white; n++) {
        v.setcolors(n, n);
        v.print("  ");
    }

    // Fuente creada en https://patorjk.com/software/taag/#p=display&h=1&f=Ogre&t=MiddleDot
    v.setcolors(vga::color::light_red, vga::color::black);
    v.gotoxy(13,10);
    v.print(R"(         _      _      _  _           ___        _)");   
    v.gotoxy(13,11);
    v.print(R"(  /\/\  (_)  __| |  __| || |  ___    /   \ ___  | |_)");
    v.gotoxy(13,12);
    v.print(R"( /    \ | | / _` | / _` || | / _ \  / /\ // _ \ | __|)");
    v.gotoxy(13,13);
    v.print(R"(/ /\/\ \| || (_| || (_| || ||  __/ / /_//| (_) || |_)");
    v.gotoxy(13,14);
    v.print(R"(\/    \/|_| \__,_| \__,_||_| \___|/___,'  \___/  \__|)");
    
    for(;;); // No puedes pasar!
}
}
