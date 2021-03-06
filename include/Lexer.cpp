#include "Lexer.hpp"


namespace mcon
{
    Lexer::Lexer(
        std::unique_ptr<CharacterStream> a_character_stream,
        std::shared_ptr<CharacterSet> a_character_set
    ):
        character_stream(std::move(a_character_stream)),
        character_set(a_character_set)
    {
        Scan();
    }

    Lexer::~Lexer()
    { }
    
    void Lexer::Scan()
    {
        // Clear any old data first
        Reset();
        
        // Create an initial token to mark the start of the token sequence
        Token temp_token(TokenType::StartOfStream);

        // Pair up the character sets and their corresponding token types for easy iteration
        std::set<std::pair<TokenType, std::set<String>>> character_sets = {
            {TokenType::EndOfStream, character_set->end_of_stream},
            {TokenType::Whitespace, character_set->whitespace},
            {TokenType::Text, character_set->letter},
            {TokenType::Number, character_set->number},
            {TokenType::Symbol, character_set->symbol}
        };

        // Main lexing loop
        while (true)
        {
            // Variable to check if the character was appended and therefore recognised
            bool character_appended = false;

            // Obtain the current character
            String current_character = character_stream->Peek();

            // Iterate over the character sets
            for (auto& set : character_sets)
            {
                // Attempt to find the current character in a set
                if (set.second.find(current_character) != set.second.end())
                {
                    // If the character type does not correspond to the current token type,
                    // or if the token is of "symbol" type, which should only hold one character,
                    // append the token to the token vector and create a new one of the correct type
                    if (temp_token.type != set.first || temp_token.type == TokenType::Symbol)
                    {
                        tokens.push_back(temp_token);
                        temp_token = Token(set.first);
                    }

                    // Append the current character to the current token
                    character_appended = temp_token.Append(character_stream->Consume());

                    // If the current token marks the end of the stream, finish lexing
                    if (temp_token.type == TokenType::EndOfStream)
                    {
                        tokens.push_back(temp_token);
                        return;
                    }
                }
            }

            // Check if the character was not found in any of the character sets
            if (!character_appended)
            {
                // If the character does not belong to a set, place it in a token of type Unknown
                tokens.push_back(temp_token);
                temp_token = Token(TokenType::Unknown);
                temp_token.Append(character_stream->Consume());

                // Report the error to the console
                ERROR_OUTPUT << STR("Unknown character: ") << current_character << STR("\n")
                << STR("The character was not found in any of the supplied character sets.\n")
                << std::endl;
            }
        }
    }
    
    void Lexer::Reset()
    {
        index = 0;
        tokens.clear();
        return;
    }

    Token Lexer::Peek(uint8_t a_offset)
    {
        return index + a_offset < tokens.size() ? tokens.at(index + a_offset) : Token(TokenType::OutOfBounds);
    }

    Token Lexer::Consume(uint8_t a_offset)
    {
        Token token = Peek(a_offset);
        index += a_offset + 1;
        return token;
    }
}