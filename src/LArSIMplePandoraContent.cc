/**
 *  @file LArSIMple/src/LArSIMplePandoraContent.cc
 * 
 *  @brief Implementation of the messenger class. 
 * 
 *  $Log: $
 */

#include "LArSIMplePandoraContent.hh"
#include "larpandoracontent/LArPersistency/EventWritingAlgorithm.h"

#include "Api/PandoraApi.h"

#define LAR_SIMPLE_PANDORA_ALGORITHM_LIST(d)                                                                                    \
    d("LArEventWriting", EventWritingAlgorithm)

#define FACTORY Factory

namespace lar_content
{

#define LAR_SIMPLE_PANDORA_CONTENT_CREATE_ALGORITHM_FACTORY(a, b)                                                               \
class b##FACTORY : public pandora::AlgorithmFactory                                                                             \
{                                                                                                                               \
public:                                                                                                                         \
    pandora::Algorithm *CreateAlgorithm() const {return new b;};                                                                \
};

LAR_SIMPLE_PANDORA_ALGORITHM_LIST(LAR_SIMPLE_PANDORA_CONTENT_CREATE_ALGORITHM_FACTORY)

//------------------------------------------------------------------------------------------------------------------------------------------

} // namespace lar_content

#define LAR_SIMPLE_PANDORA_CONTENT_REGISTER_ALGORITHM(a, b)                                                                     \
{                                                                                                                               \
    const pandora::StatusCode statusCode(PandoraApi::RegisterAlgorithmFactory(pandora, a, new lar_content::b##FACTORY));        \
    if (pandora::STATUS_CODE_SUCCESS != statusCode)                                                                             \
        return statusCode;                                                                                                      \
}

#define LAR_SIMPLE_PANDORA_CONTENT_REGISTER_ALGORITHM_TOOL(a, b)                                                                \
{                                                                                                                               \
    const pandora::StatusCode statusCode(PandoraApi::RegisterAlgorithmToolFactory(pandora, a, new lar_content::b##FACTORY));    \
    if (pandora::STATUS_CODE_SUCCESS != statusCode)                                                                             \
        return statusCode;                                                                                                      \
}

pandora::StatusCode LArSIMplePandoraContent::RegisterAlgorithms(const pandora::Pandora &pandora)
{
    LAR_SIMPLE_PANDORA_ALGORITHM_LIST(LAR_SIMPLE_PANDORA_CONTENT_REGISTER_ALGORITHM);
    return pandora::STATUS_CODE_SUCCESS;
}

