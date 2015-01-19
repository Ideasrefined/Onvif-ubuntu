
#include "MediaTypes.h"
#ifdef MEDIA_S
#include "commonTypes.h"

#include "MediaServiceImpl.h"


char MedCellLayout::encoding_table[] = {'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H',
                                        'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P',
                                        'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X',
                                        'Y', 'Z', 'a', 'b', 'c', 'd', 'e', 'f',
                                        'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n',
                                        'o', 'p', 'q', 'r', 's', 't', 'u', 'v',
                                        'w', 'x', 'y', 'z', '0', '1', '2', '3',
                                        '4', '5', '6', '7', '8', '9', '+', '/'};

int MedCellLayout::mod_table[] = {0, 2, 1};

MedCellLayout::MedCellLayout( int rows, int cols,
                              CellDetectionLayout::Fill fill ):
                              decoding_table_(NULL) {
    int height = rows;
    pitch_ = cols / 8 + ( cols % 8 == 0 ? 0 : 1 );
    size_ = height * pitch_;
    layout_ = new char[ size_ ];
    memset(layout_, 0, size_);

    int centerLeft = cols * 0.25;
    int centerRight = cols * 0.75;
    int centerBottom = rows * 0.25;
    int centerTop = rows * 0.75;
    for( int i = 0; i < rows; ++i ) {
        for( int j = 0; j < cols; ++j ) {
            if( fill == CellDetectionLayout::ALL ||
                ( fill == CellDetectionLayout::CENTER &&
                i > centerBottom && i < centerTop &&
                j > centerLeft && j <= centerRight ) )
                setAt(i, j, 255);
        }
    }
}

MedCellLayout::MedCellLayout( const std::string base64EncLayout ):
    layout_(NULL),
    decoding_table_(NULL),
    pitch_(0),
    size_(0) {
    setLayout(base64EncLayout);
}

MedCellLayout::MedCellLayout():
    layout_(NULL),
    decoding_table_(NULL),
    pitch_(0),
    size_(0) {
}

MedCellLayout::~MedCellLayout() {
   if( layout_ )
       delete layout_;
}

std::string
MedCellLayout::getBase64Encoded() {
    std::string rez0 = packBitsEncoding( reinterpret_cast<const char*>(layout_), size_ );
    size_t enc_size;
    char* encoded = base64_encode( reinterpret_cast<const unsigned char*>( rez0.c_str() ), rez0.size(), &enc_size );
    std::string out(encoded);

    if(encoded)
        delete encoded;
    return out;
}

bool
MedCellLayout::setLayout( const std::string & base64EncLayout ) {
    size_t decSize;
    unsigned char* decoded = base64_decode( base64EncLayout.c_str(),
                                            base64EncLayout.size(), &decSize );
    if(!decoded)
        return false;

    packBitsDecoding( reinterpret_cast<const char*>(decoded), decSize );
    delete decoded;
    return true;
}

CellDetectionLayout::Fill
MedCellLayout::getFill( int rows, int cols ) const {
    bool all = true;
    for( int i = 0; i < rows; ++i ) {
        for( int j = 0; j < cols; ++j ) {
            if( getAt(i,j) != 0 ) {
                continue;
            } else {
                all = false;
                break;
            }
        }
    }
    return all ? CellDetectionLayout::ALL : CellDetectionLayout::CENTER;
}

char
MedCellLayout::getAt( int y, int x) const {
    int offset = y * pitch_ + x / 8;
    int mask;
    switch( x % 8 ) {
        case 0:
            mask = 0x1;
        break;
        case 1:
            mask = 0x2;
        break;
        case 2:
            mask = 0x4;
        break;
        case 3:
            mask = 0x8;
        break;
        case 4:
            mask = 0x10;
        break;
        case 5:
            mask = 0x20;
        break;
        case 6:
            mask = 0x40;
        break;
        case 7:
            mask = 0x80;
        break;
    }
    char value = layout_[offset] & mask;
    return value;
}

void
MedCellLayout::setAt( int y, int x,
                      char value ) {
    int offset = y * pitch_ + x / 8;
    int mask;
    switch( x % 8 ) {
        case 0:
            mask = 0x1;
        break;
        case 1:
            mask = 0x2;
        break;
        case 2:
            mask = 0x4;
        break;
        case 3:
            mask = 0x8;
        break;
        case 4:
            mask = 0x10;
        break;
        case 5:
            mask = 0x20;
        break;
        case 6:
            mask = 0x40;
        break;
        case 7:
            mask = 0x80;
        break;
    }

    layout_[offset] |= value & mask;
}

std::string
MedCellLayout::packBitsEncoding(const char* input, size_t size) {
    std::string output;
    bool onTheRun = false;
    bool onTheCopyRun = false;
    int runSizeIndex;
    int copyRunSizeIndex;
    for(int i = 0; i < size; ++i ) {
        if( onTheRun && ( input[i] != *output.rbegin()) ) {
            onTheRun = false;
        }

        if( onTheRun && input[i] == *output.rbegin() ) {
            output[runSizeIndex] = output[runSizeIndex] - 1;
        } else if( i != size - 1 && input[i] == input[i+1] ) {
            onTheRun = true;
            onTheCopyRun = false;
            output.push_back(0);
            runSizeIndex = output.size() - 1;
            output.push_back( input[i] );
        } else if( onTheCopyRun ) {
            output[copyRunSizeIndex] = output[copyRunSizeIndex] + 1;
            output.push_back(input[i]);
        } else {
            onTheCopyRun = true;
            output.push_back(0);
            copyRunSizeIndex = output.size() - 1;
            output.push_back(input[i]);
        }
    }

    return output;
}

void
MedCellLayout::packBitsDecoding(const char *input, size_t size) {
    std::string output;
    int i = 0;
    while(i < size) {
        if( input[i] == -128 ) {
            ++i;
            continue;
        } else if( input[i] < 0 ) {
            int chunk = 1 - input[i];
            std::string s( chunk, input[i+1]);
            output.append(s);
            i += 2;
        } else if( input[i] >= 0 ) {
            int chunk = input[i] + 1;
            ++i;
            for( int j = i; j < i + chunk; ++j ) {
                output.push_back(input[j]);
            }
            i += chunk;
        }
    }

    size_ = output.size();
    if(layout_)
        delete[] layout_;
    layout_ = new char [ size_ ];
    memcpy( layout_, output.c_str(), size_ );
}

unsigned char *
MedCellLayout::base64_decode(const char *data,
                             size_t input_length,
                             size_t *output_length) {

    if (decoding_table_ == NULL) build_decoding_table();

    if (input_length % 4 != 0) return NULL;

    *output_length = input_length / 4 * 3;
    if (data[input_length - 1] == '=') (*output_length)--;
    if (data[input_length - 2] == '=') (*output_length)--;

    unsigned char *decoded_data = static_cast<unsigned char*>( malloc(*output_length) );
    if (decoded_data == NULL) return NULL;

    for (int i = 0, j = 0; i < input_length;) {

        uint32_t sextet_a = data[i] == '=' ? 0 & i++ : decoding_table_[data[i++]];
        uint32_t sextet_b = data[i] == '=' ? 0 & i++ : decoding_table_[data[i++]];
        uint32_t sextet_c = data[i] == '=' ? 0 & i++ : decoding_table_[data[i++]];
        uint32_t sextet_d = data[i] == '=' ? 0 & i++ : decoding_table_[data[i++]];

        uint32_t triple = (sextet_a << 3 * 6)
        + (sextet_b << 2 * 6)
        + (sextet_c << 1 * 6)
        + (sextet_d << 0 * 6);

        if (j < *output_length) decoded_data[j++] = (triple >> 2 * 8) & 0xFF;
        if (j < *output_length) decoded_data[j++] = (triple >> 1 * 8) & 0xFF;
        if (j < *output_length) decoded_data[j++] = (triple >> 0 * 8) & 0xFF;
    }

    return decoded_data;
}


void
MedCellLayout::build_decoding_table() {
    decoding_table_ = static_cast<char*>( malloc(256) );
    for (int i = 0; i < 64; i++)
        decoding_table_[(unsigned char) encoding_table[i]] = i;
}

char *
MedCellLayout::base64_encode( const unsigned char *data,
                              size_t input_length,
                              size_t *output_length) {
    *output_length = 4 * ((input_length + 2) / 3);

    char *encoded_data = static_cast<char*>( malloc(*output_length) );
    if (encoded_data == NULL) return NULL;

    for (int i = 0, j = 0; i < input_length;) {

        uint32_t octet_a = i < input_length ? data[i++] : 0;
        uint32_t octet_b = i < input_length ? data[i++] : 0;
        uint32_t octet_c = i < input_length ? data[i++] : 0;

        uint32_t triple = (octet_a << 0x10) + (octet_b << 0x08) + octet_c;

        encoded_data[j++] = encoding_table[(triple >> 3 * 6) & 0x3F];
        encoded_data[j++] = encoding_table[(triple >> 2 * 6) & 0x3F];
        encoded_data[j++] = encoding_table[(triple >> 1 * 6) & 0x3F];
        encoded_data[j++] = encoding_table[(triple >> 0 * 6) & 0x3F];
    }

    for (int i = 0; i < mod_table[input_length % 3]; i++)
        encoded_data[*output_length - 1 - i] = '=';

    return encoded_data;
}

///////////////////////////////////////////////////////////////////////////////////////
MedVideoAnalyticsConfiguration::MedVideoAnalyticsConfiguration( tt__VideoAnalyticsConfiguration* p ) {
    d = p;
}

std::vector<class tt__Config * >
MedVideoAnalyticsConfiguration::getAnalyticsModules() {
    return d->AnalyticsEngineConfiguration->AnalyticsModule;
}

MedVideoAnalyticsConfiguration::MedVideoAnalyticsConfiguration( soap* pSoap,
                                                                const std::string& name,
                                                                const std::string& token,
                                                                CellDetectionLayout::Fill fill ) {
    d = soap_new_tt__VideoAnalyticsConfiguration(pSoap, -1);
    d->token = token;
    d->Name = name;
    d->AnalyticsEngineConfiguration = soap_new_tt__AnalyticsEngineConfiguration(pSoap, -1);
    d->AnalyticsEngineConfiguration->AnalyticsModule.push_back(soap_new_tt__Config(pSoap, -1));
    d->AnalyticsEngineConfiguration->AnalyticsModule.back()->Name = "CellMotion";
    d->AnalyticsEngineConfiguration->AnalyticsModule.back()->Type = "tt:CellMotionEngine";
    d->AnalyticsEngineConfiguration->AnalyticsModule.back()->Parameters = soap_new_tt__ItemList(pSoap, -1);

    d->AnalyticsEngineConfiguration->AnalyticsModule.back()->Parameters->SimpleItem =
            new std::vector<_tt__ItemList_SimpleItem>();
    d->AnalyticsEngineConfiguration->AnalyticsModule.back()->Parameters->ElementItem =
            new std::vector<_tt__ItemList_ElementItem>();

    std::vector<_tt__ItemList_SimpleItem>* simples = d->AnalyticsEngineConfiguration->AnalyticsModule.back()->Parameters->SimpleItem;
    std::vector<_tt__ItemList_ElementItem>* elems = d->AnalyticsEngineConfiguration->AnalyticsModule.back()->Parameters->ElementItem;
    simples->push_back(_tt__ItemList_SimpleItem());
    simples->back().Name = "Sensitivity";
    simples->back().Value = "50";
    elems->push_back(_tt__ItemList_ElementItem());
    elems->back().Name = "Layout";
    elems->back().Layout = soap_new_tt__CellLayout(pSoap, -1);
    elems->back().Layout->Columns = "4";
    elems->back().Layout->Rows = "3";

    d->RuleEngineConfiguration = soap_new_tt__RuleEngineConfiguration(pSoap, -1);
    d->RuleEngineConfiguration->Rule.push_back( soap_new_tt__Config(pSoap, -1) );
    tt__Config* rule = d->RuleEngineConfiguration->Rule.back();
    rule->Name = "CellMotionDetection";
    rule->Type = "tt:CellMotionDetector";
    if( fill == CellDetectionLayout::CENTER )
        rule->Name.append("_CenterOnly");
    rule->Parameters = soap_new_tt__ItemList(pSoap, -1);
    MedCellLayout cAll( 3, 4, fill);
    std::string s = cAll.getBase64Encoded();
    rule->Parameters->SimpleItem = new std::vector<_tt__ItemList_SimpleItem>();
    rule->Parameters->SimpleItem->push_back(_tt__ItemList_SimpleItem());
    rule->Parameters->SimpleItem->back().Name = "MinCount";
    rule->Parameters->SimpleItem->back().Value = "1";
    rule->Parameters->SimpleItem->push_back(_tt__ItemList_SimpleItem());
    rule->Parameters->SimpleItem->back().Name = "AlarmOnDelay";
    rule->Parameters->SimpleItem->back().Value = "1000";
    rule->Parameters->SimpleItem->push_back(_tt__ItemList_SimpleItem());
    rule->Parameters->SimpleItem->back().Name = "AlarmOffDelay";
    rule->Parameters->SimpleItem->back().Value = "1000";
    rule->Parameters->SimpleItem->push_back(_tt__ItemList_SimpleItem());
    rule->Parameters->SimpleItem->back().Name = "ActiveCells";
    rule->Parameters->SimpleItem->back().Value = s;
}

std::string
MedVideoAnalyticsConfiguration::getToken() const {
    return d->token;
}

bool
MedVideoAnalyticsConfiguration::getFill( CellDetectionLayout::Fill & fill ) const {
    if( d->AnalyticsEngineConfiguration == NULL ||
        d->AnalyticsEngineConfiguration->AnalyticsModule.empty() )
        return false;

    std::vector<_tt__ItemList_ElementItem>* elems = d->AnalyticsEngineConfiguration->AnalyticsModule.back()->Parameters->ElementItem;
    if( elems == NULL )
        return false;
    int cols = atoi( elems->back().Layout->Columns.c_str() );
    int rows = atoi( elems->back().Layout->Rows.c_str() );

    if( d->RuleEngineConfiguration == NULL ||
        d->RuleEngineConfiguration->Rule.empty() )
        return false;
    std::vector<_tt__ItemList_SimpleItem>* simple = d->RuleEngineConfiguration->Rule.back()->Parameters->SimpleItem;
    if( simple == NULL )
        return false;
    std::string activeCells("");
    for( std::vector<_tt__ItemList_SimpleItem>::const_iterator it = simple->begin();
         it != simple->end(); ++it ) {
        if((*it).Name == "ActiveCells") {
            activeCells = (*it).Value;
            break;
        }
    }

    if(activeCells.empty())
        return false;

    MedCellLayout layout(activeCells);
    fill = layout.getFill(rows, cols);
    return true;
}

///////////////////////////////////////////////////////////////////////////////////////
#define EXTRA_CONSTRUCT() \
{\
}

CLASS_CTORS(trt, Med, GetProfileResponse)
CLASS_CTORS(trt, Med, GetProfilesResponse)
CLASS_CTORS(trt, Med, GetVideoSourcesResponse)
CLASS_CTORS(trt, Med, GetCompatibleVideoEncoderConfigurationsResponse)
CLASS_CTORS(trt, Med, GetCompatibleVideoAnalyticsConfigurationsResponse)

void
MedGetVideoSourcesResponse::AddEntry( const std::string & sourceToken,
                                      int w, int h, float frmRate ) {

	d->VideoSources.push_back( soap_new_tt__VideoSource( d->soap, -1 ) );
    d->VideoSources.back()->token = sourceToken;
    d->VideoSources.back()->Framerate = frmRate;
    d->VideoSources.back()->Resolution = soap_new_tt__VideoResolution(d->soap, -1);
    d->VideoSources.back()->Resolution->Width = w;
    d->VideoSources.back()->Resolution->Height = h;
}

void
MedGetVideoSourcesResponse::AddEntry( const MedVideoSource & src ) {
    d->VideoSources.push_back( src.d );
}

void
MedGetProfileResponse::SetProfile( MedProfile& prof ) {
    d->Profile = prof.d;
}

void
MedGetProfilesResponse::AddProfile( MedProfile& prof ) {
    d->Profiles.push_back(prof.d);
}

void MedGetProfilesResponse::GetProfilesTokens(std::string tokens[])
{
	int profiles_found = d->Profiles.size();
	if(profiles_found > 4)
	{
		printf("Cannot support more than 4 profiles. %s -> %s \n",__FILE__,__FUNCTION__);
		return;
	}

	for(int i = 0 ; i < profiles_found; i++)
	{
		tokens[i] = d->Profiles[i]->token;
		//printf("PRF:%d -> TK:%s\n->NM:%s\n",i,resp.d->Profiles[i]->token.c_str(),resp.d->Profiles[i]->Name.c_str());
	}
}

MedProfile::MedProfile( tt__Profile* pData) {
    d = pData;
}

MedProfile::MedProfile( soap * pSoap, const std::string& name, const std::string& token ) {
    d = soap_new_tt__Profile(pSoap);
    d->Name = name;
    d->token = token;
    d->fixed = static_cast<bool*>(soap_malloc(pSoap, sizeof(bool)));
    *d->fixed = false;
}

void
MedProfile::AddVideoSrc(const std::string & sourceToken, int w, int h) {
    d->VideoSourceConfiguration = soap_new_tt__VideoSourceConfiguration(d->soap, -1);
    d->VideoSourceConfiguration->SourceToken = sourceToken;
    d->VideoSourceConfiguration->Bounds = soap_new_tt__IntRectangle(d->soap, -1);
    d->VideoSourceConfiguration->Bounds->x = 0;
    d->VideoSourceConfiguration->Bounds->y = 0;
    d->VideoSourceConfiguration->Bounds->width = w;
    d->VideoSourceConfiguration->Bounds->height = h;

}

void
MedProfile::AddVideoEnc( int w, int h ) {
    d->VideoEncoderConfiguration = soap_new_tt__VideoEncoderConfiguration(d->soap, -1);
    d->VideoEncoderConfiguration->Encoding =  tt__VideoEncoding__H264;
    d->VideoEncoderConfiguration->Quality = 100.0f;
    d->VideoEncoderConfiguration->Resolution = soap_new_tt__VideoResolution(d->soap, -1);
    d->VideoEncoderConfiguration->Resolution->Width = w;
    d->VideoEncoderConfiguration->Resolution->Height = h;
}

const std::string &
MedProfile::getToken() {
    return d->token;
}

const std::string &
MedProfile::getVAName() {
    return d->VideoAnalyticsConfiguration->Name;
}

MedVideoAnalyticsConfiguration
MedProfile:: getVideoAnalytics() const {
    MedVideoAnalyticsConfiguration va(d->VideoAnalyticsConfiguration);
    return va;
}

void
MedProfile::AddVideoAnalytics( MedVideoAnalyticsConfiguration & an ) {
    d->VideoAnalyticsConfiguration = an.d;
}

MedVideoSource::MedVideoSource( soap * pSoap, const std::string& token,
                                int w, int h, double frmRate) {
    d = soap_new_tt__VideoSource( pSoap );
    d->token = token;
    d->Framerate = frmRate;
    d->Resolution = soap_new_tt__VideoResolution( pSoap );
    d->Resolution->Width = w;
    d->Resolution->Height = h;
}

const std::string &
MedVideoSource::getToken() {
    return d->token;
}

void
MedGetCompatibleVideoEncoderConfigurationsResponse::AddVideoEnc( int w, int h ) {
    d->Configurations.push_back( soap_new_tt__VideoEncoderConfiguration(d->soap, -1) );
    d->Configurations.back()->Encoding =  tt__VideoEncoding__H264;
    d->Configurations.back()->Quality = 100.0f;
    d->Configurations.back()->Resolution = soap_new_tt__VideoResolution(d->soap, -1);
    d->Configurations.back()->Resolution->Width = w;
    d->Configurations.back()->Resolution->Height = h;
}

void
MedGetCompatibleVideoAnalyticsConfigurationsResponse::AddVideoAn( MedVideoAnalyticsConfiguration & conf ) {
    d->Configurations.push_back( conf.d );
}

#endif //MEDIA_S
