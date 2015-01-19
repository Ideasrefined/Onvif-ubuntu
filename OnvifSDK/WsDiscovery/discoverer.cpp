
#include "discoverer.h"

Discoverer::Discoverer() {
}

Discoverer::~Discoverer() {
}

std::vector<DiscoveryMatch>
Discoverer::getMembers( const std::string & onvifDeviceType )
{
    search( onvifDeviceType );
    return members_;
}

bool
Discoverer::collectNewMatches( const ProbeMatches_t matches) {
    bool collectedSomeMatches = false;
    for( ProbeMatches_t::const_iterator currentProbe = matches.begin();
        currentProbe != matches.end(); ++currentProbe ) {
        bool newMatch = true;
        for( std::vector<DiscoveryMatch>::const_iterator it = members_.begin();
            it != members_.end(); ++it ) {
            if( it->xaddr_ == *currentProbe->xaddrs ) {
                newMatch = false;
                break;
            }
        }

        if( newMatch ) {
            members_.push_back( DiscoveryMatch( *currentProbe->types,
                                                *currentProbe->xaddrs,
                                                currentProbe->scopes->item ) );
            collectedSomeMatches = true;
        }
    }

    return collectedSomeMatches;
}

void Discoverer::search( const std::string & type ) {
    try {
        std::string aType(type);
        Probe_t justProbe;
        justProbe.scopes = NULL;
        justProbe.types = &aType;
        int failAttemptsCounter = 0;

        proxy_.reset(RemoteDiscovery::proxy());
        while( true ) {
            if( failAttemptsCounter == probeAttemptsMaxAmount_ )
                break;

            ProbeMatches_t matches = proxy_->probe( justProbe );

            if( !collectNewMatches( matches ) )
                ++failAttemptsCounter;

            #warning REWRITE Dirty implementation,
            // but Wsdd achitecture and gsoap stubs force us
            // to dynamic allocs using and passing vector object through return
            for( ProbeMatches_t::iterator it = matches.begin();
                 it != matches.end(); ++it ) {
                    if( it->xaddrs ) delete it->xaddrs;
                    if( it->types ) delete it->types;
                    if( it->scopes ) delete it->scopes;
            }
        }
    } catch (std::exception & ex) {
        SIGRLOG( SIGRDEBUG2, "[runClientService] Caught an exception: %s", ex.what() );
    }
}
