#pragma once
#ifndef SLARX_CONVERSION_NFA_H_INCLUDED
#define SLARX_CONVERSION_NFA_H_INCLUDED

namespace slarx
{
    // This is a utility class, which is to be used when reading an
    // automaton of unknown type (or a known NFA) or when performing
    // operations on a DFA, which produce an NFA (such as union,
    // concatenation, etc). Its only purpose is to be converted to a
    // DFA and should not be used for anything else
    class ConversionNFA
    {

    };
}

#endif // SLARX_CONVERSION_NFA_H_INCLUDED