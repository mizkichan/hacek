#ifndef DEFS_H
#define DEFS_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

struct Position {
  char *file;
  size_t line;
  size_t column;
};

enum Keyword {
  ALIGNAS,
  ALIGNOF,
  ATOMIC,
  AUTO,
  BOOL,
  BREAK,
  CASE,
  CHAR,
  COMPLEX,
  CONST,
  CONTINUE,
  DEFAULT,
  DO,
  DOUBLE,
  ELSE,
  ENUM,
  EXTERN,
  FLOAT,
  FOR,
  GENERIC,
  GOTO,
  IF,
  IMAGINARY,
  INLINE,
  INT,
  LONG,
  NORETURN,
  REGISTER,
  RESTRICT,
  RETURN,
  SHORT,
  SIGNED,
  SIZEOF,
  STATIC,
  STATIC_ASSERT,
  STRUCT,
  SWITCH,
  THREAD_LOCAL,
  TYPEDEF,
  UNION,
  UNSIGNED,
  VOID,
  VOLATILE,
  WHILE,
};

struct IntegerConstant {
  struct Position pos;
  enum IntegerConstantType {
    INTEGER_CONSTANT_INT,
    INTEGER_CONSTANT_UINT,
    INTEGER_CONSTANT_LONG,
    INTEGER_CONSTANT_ULONG,
    INTEGER_CONSTANT_LLONG,
    INTEGER_CONSTANT_ULLONG,
  } type;
  union {
    int value_int;
    unsigned int value_uint;
    long value_long;
    unsigned long value_ulong;
    long long value_llong;
    unsigned long long value_ullong;
  };
};

struct Constant {
  enum ConstantKind {
    INTEGER_CONSTANT,
    FLOATING_CONSTANT,
    ENUMERATION_CONSTANT,
    CHARACTER_CONSTANT,
  } kind;
  union {
    struct IntegerConstant *integer_constant;
    struct FloatingConstant *floating_constant;
    struct EnumerationConstant *enumeration_constant;
    struct CharacterConstant *character_constant;
  };
};

struct Token {
  enum TokenKind {
    TOKEN_KEYWORD,
    TOKEN_IDENTIFIER,
    TOKEN_CONSTANT,
    TOKEN_STRING_LITERAL,
    TOKEN_PUNCTUATOR,
  } kind;
  union {
    enum Keyword keyword;
    struct Identifier *identifier;
    struct Constant *constant;
    struct StringLiteral *string_literal;
    struct Punctuator *punctuator;
  };
};

struct HeaderName {
  struct Position pos;
  enum HeaderNameKind { H_CHAR_SEQUENCE, Q_CHAR_SEQUENCE } kind;
  char value[];
};

struct Identifier {
  struct Position pos;
  char value[];
};

struct PPNumber {
  struct Position pos;
  char value[];
};

struct CharacterConstant {
  struct Position pos;
  enum CharacterConstantPrefix {
    CHARACTER_CONSTANT_PREFIX_WCHAR,
    CHARACTER_CONSTANT_PREFIX_CHAR16,
    CHARACTER_CONSTANT_PREFIX_CHAR32,
    CHARACTER_CONSTANT_PREFIX_NONE
  } prefix;
  char value[];
};

struct StringLiteral {
  struct Position pos;
  enum StringLiteralPrefix {
    STRING_LITERAL_PREFIX_UTF8,
    STRING_LITERAL_PREFIX_CHAR16,
    STRING_LITERAL_PREFIX_CHAR32,
    STRING_LITERAL_PREFIX_WCHAR,
    STRING_LITERAL_PREFIX_NONE
  } prefix;
  char value[];
};

struct Punctuator {
  struct Position pos;
  enum PunctuatorKind {
    LEFT_BRACKET,          // [
    RIGHT_BRACKET,         // ]
    LEFT_PAREN,            // (
    RIGHT_PAREN,           // )
    LEFT_BRACE,            // {
    RIGHT_BRACE,           // }
    DOT,                   // .
    ARROW,                 // ->
    INCREMENT,             // ++
    DECREMENT,             // --
    AMPASAND,              // &
    ASTERISK,              // *
    PLUS,                  // +
    MINUS,                 // -
    NEGATE,                // ~
    EXCLAMATION,           // !
    DIVIDE,                // /
    REMIND,                // %
    LEFT_SHIFT,            // <<
    RIGHT_SHIFT,           // >>
    LESS_THAN,             // <
    GREATER_THAN,          // >
    LESS_EQUAL,            // <=
    GREATER_EQUAL,         // >=
    EQUAL,                 // ==
    NOT_EQUAL,             // !=
    EXCLUSIVE_OR,          // ^
    INCLUSIVE_OR,          // |
    LOGICAL_AND,           // &&
    LOGICAL_OR,            // ||
    QUESTION,              // ?
    COLON,                 // :
    SEMICOLON,             // ;
    ELLIPSE,               // ...
    ASSIGN,                // =
    MULTIPLY_ASSIGN,       // *=
    DIVIDE_ASSIGN,         // /=
    REMIND_ASSIGN,         // %=
    ADD_ASSIGN,            // +=
    SUBTRACT_ASSIGN,       // -=
    LEFT_SHIFT_ASSIGN,     // <<=
    RIGHT_SHIFT_ASSIGN,    // >>=
    AND_ASSIGN,            // &=
    EXCLUSIVE_OR_ASSIGN,   // ^=
    INCLUSIVE_OR_ASSIGN,   // |=
    COMMA,                 // ,
    SIGN,                  // #
    DOUBLE_SIGN,           // ##
    DIGRAPH_LEFT_BRACKET,  // <:
    DIGRAPH_RIGHT_BRACKET, // :>
    DIGRAPH_LEFT_BRACE,    // <%
    DIGRAPH_RIGHT_BRACE,   // %>
    DIGRAPH_SIGN,          // %:
    DIGRAPH_DOUBLE_SIGN,   // %:%:
  } kind;
};

struct PPToken {
  enum PPTokenKind {
    PP_HEADER_NAME,
    PP_IDENTIFIER,
    PP_NUMBER,
    PP_CHARACTER_CONSTANT,
    PP_STRING_LITERAL,
    PP_PUNCTUATOR,
  } kind;
  union {
    struct HeaderName *header_name;
    struct Identifier *identifier;
    struct PPNumber *number;
    struct CharacterConstant *character_constant;
    struct StringLiteral *string_literal;
    struct Punctuator *punctuator;
  };
};

struct Line {
  size_t linenum;
  char *value;
};

struct PPTokenLine {
  size_t linenum;
  struct PPToken **pp_tokens;
};

/*
 * Phrase structures.
 * See N2176 A.2
 */

struct PrimaryExpression {
  enum PrimaryExpressionKind {
    PE_IDENTIFIER,
    PE_CONSTANT,
    PE_STRING_LITERAL,
    PE_EXPRESSION,
    PE_GENERIC_SELECTION,
  } kind;
  union {
    struct Identifier *identifier;
    struct Constant *constant;
    struct StringLiteral *string_literal;
    struct Expression *expression;
    struct GenericSelection *generic_selection;
  };
};

struct GenericSelection;
struct GenericAssocList;
struct GenericAssociation;

struct PostfixExpression {
  enum PostfixExpressionKind {
    POSTFIX_PRIMARY_EXPRESSION,
    POSTFIX_ARRAY_SUBSCRIPTING,
    POSTFIX_FUNCTION_CALL,
    POSTFIX_MEMBER,
    POSTFIX_POINTER_MEMBER,
    POSTFIX_INCREMENT,
    POSTFIX_DECREMENT,
    POSTFIX_COMPOUND_LITERAL,
  } kind;
  union {
    struct PrimaryExpression *primary_expression;
    struct {
      struct PostfixExpression *postfix_expression;
      union {
        struct Expression *expression;
        struct ArgumentExpressionList *argument_expression_list;
        struct Identifier *identifier;
      };
    };
    struct {
      struct TypeName *type_name;
      struct InitializerList *initializer_list;
    };
  };
};

// DO NOT USE. USE `struct ArgumentExpression**` INSTEAD.
struct ArgumentExpressionList;

struct UnaryExpression {
  enum UnaryExpressionKind {
    UE_POSTFIX_EXPRESSION,
    UE_INCREMENT,
    UE_DECREMENT,
    UE_SIZEOF_EXPRESSION,
    UE_SIZEOF_TYPE,
    UE_ALIGNOF,

    UE_REFERENCE,
    UE_DEREFERENCE,
    UE_PLUS,
    UE_MINUS,
    UE_NEGATE,
    UE_NOT,
  } kind;
  union {
    struct PostfixExpression *postfix_expression;
    struct UnaryExpression *unary_expression;
    struct CastExpression *cast_expression;
    struct TypeName *type_name;
  };
};

struct CastExpression {
  bool is_unary_expression : 1;
  union {
    struct UnaryExpression *unary_expression;
    struct {
      struct TypeName *type_name;
      struct CastExpression *cast_expression;
    };
  };
};

struct MultiplicativeExpression {
  enum MultiplicativeExpressionKind {
    ME_CAST_EXPRESSION,
    ME_MULTIPLY,
    ME_DIVIDE,
    ME_REMIND,
  } kind;
  union {
    struct CastExpression *cast_expression;
    struct {
      struct MultiplicativeExpression *lhs;
      struct CastExpression *rhs;
    };
  };
};

struct AdditiveExpression {
  enum AdditiveExpressionKind {
    AE_MULTIPLICATIVE_EXPRESSION,
    AE_ADD,
    AE_SUBTRACT,
  } kind;
  union {
    struct MultiplicativeExpression *multiplicative_expression;
    struct {
      struct AdditiveExpression *lhs;
      struct MultiplicativeExpression *rhs;
    };
  };
};

struct ShiftExpression {
  enum ShiftExpressionKind {
    SE_ADDITIVE_EXPRESSION,
    SE_LEFT,
    SE_RIGHT,
  } kind;
  union {
    struct AdditiveExpression *additive_expression;
    struct {
      struct ShiftExpression *lhs;
      struct AdditiveExpression *rhs;
    };
  };
};

struct RelationalExpression {
  enum RelationalExpressionKind {
    RE_SHIFT_EXPRESSION,
    RE_LESS_THAN,
    RE_GREATER_THAN,
    RE_LESS_EQUAL,
    RE_GREATER_EQUAL,
  } kind;
  union {
    struct ShiftExpression *shift_expression;
    struct {
      struct RelationalExpression *lhs;
      struct ShiftExpression *rhs;
    };
  };
};

struct EqualityExpression {
  enum EqualityExpressionKind {
    EE_RELATIONAL_EXPRESSION,
    EE_EQUAL,
    EE_NOT_EQUAL,
  } kind;
  union {
    struct RelationalExpression *relational_expression;
    struct {
      struct EqualityExpression *lhs;
      struct RelationalExpression *rhs;
    };
  };
};

struct ANDExpression {
  bool is_equality_expression : 1;
  union {
    struct EqualityExpression *equality_expression;
    struct {
      struct ANDExpression *lhs;
      struct EqualityExpression *rhs;
    };
  };
};

struct ExclusiveORExpression {
  bool is_and_expression : 1;
  union {
    struct ANDExpression *and_expression;
    struct {
      struct ExclusiveORExpression *lhs;
      struct ANDExpression *rhs;
    };
  };
};

struct InclusiveORExpression {
  bool is_exclusive_or_expression : 1;
  union {
    struct ExclusiveORExpression *exclusive_or_expression;
    struct {
      struct InclusiveORExpression *lhs;
      struct ExclusiveORExpression *rhs;
    };
  };
};

struct LogicalANDExpression {
  bool is_inclusive_or_expression : 1;
  union {
    struct InclusiveORExpression *inclusive_or_expression;
    struct {
      struct LogicalANDExpression *lhs;
      struct InclusiveORExpression *rhs;
    };
  };
};

struct LogicalORExpression {
  bool is_logical_and_expression : 1;
  union {
    struct LogicalANDExpression *logical_and_expression;
    struct {
      struct LogicalORExpression *lhs;
      struct LogicalANDExpression *rhs;
    };
  };
};

struct ConditionalExpression {
  bool is_logical_or_expression : 1;
  union {
    struct LogicalORExpression *logical_or_expression;
    struct {
      struct LogicalORExpression *condition;
      struct Expression *lhs;
      struct ConditionalExpression *rhs;
    };
  };
};

struct AssignmentExpression {
  enum AssignmentExpressionKind {
    ASSIGNMENT_SIMPLE,
    ASSIGNMENT_MULTIPLY,
    ASSIGNMENT_DIVIDE,
    ASSIGNMENT_REMIND,
    ASSIGNMENT_ADD,
    ASSIGNMENT_SUBTRACT,
    ASSIGNMENT_LEFT_SHIFT,
    ASSIGNMENT_RIGHT_SHIFT,
    ASSIGNMENT_AND,
    ASSIGNMENT_EXCLUSIVE_OR,
    ASSIGNMENT_INCLUSIVE_OR,
  } kind;
  union {
    struct ConditionalExpression *conditional_expression;
    struct {
      struct UnaryExpression *lhs;
      struct AssignmentExpression *rhs;
    };
  };
};

// DO NOT USE. USE `struct AssignmentExpression**` INSTEAD.
struct Expression;

// Does this really needed?
struct ConstantExpression;

#endif
// vim: set ft=c ts=2 sw=2 et:
