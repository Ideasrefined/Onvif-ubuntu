
#include "OnvifSDK.h"

#ifdef MEDIA_S
#include <sstream>

class MedCellLayout {
public:
    MedCellLayout( int rows, int cols,
                   CellDetectionLayout::Fill fill );
    MedCellLayout( const std::string base64EncLayout );
    MedCellLayout();
    ~MedCellLayout();

    std::string getBase64Encoded();
    bool setLayout( const std::string & base64EncLayout );
    CellDetectionLayout::Fill getFill( int rows, int cols ) const;

private:
    void setAt( int y, int x, char value );
    char getAt( int y, int x) const;
    std::string packBitsEncoding( const char* plain, size_t sz );
    void packBitsDecoding( const char *input, size_t size );
    unsigned char * base64_decode( const char *data,
                                   size_t input_length,
                                   size_t *output_length );
    void build_decoding_table();
    char * base64_encode( const unsigned char *data,
                          size_t input_length,
                          size_t *output_length);

    size_t size_;
    int pitch_;
    char * layout_;
    char *decoding_table_;
    static int mod_table[];
    static char encoding_table[];
};

#endif //MEDIA_S
