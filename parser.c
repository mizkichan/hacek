#include "alloc.h"
#include "defs.h"
#include "error.h"
#include "parser.h"
#include "token.h"
#include "utils.h"

static void next_token(const struct Token ***);
static void restore_position_if(bool, const struct Token ***,
                                const struct Token **);
// static bool accept_token(struct Token ***, enum TokenKind);
// static bool expect_token(struct Token ***, enum TokenKind);
static bool accept_punctuator(const struct Token ***, enum PunctuatorKind);
static bool expect_punctuator(const struct Token ***, enum PunctuatorKind);

static struct PrimaryExpression *
parse_primary_expression(const struct Token ***);
static struct GenericSelection *parse_generic_selection(const struct Token ***);
static struct GenericAssocList *
parse_generic_assoc_list(const struct Token ***);
static struct GenericAssociation *
parse_generic_association(const struct Token ***);
static struct PostfixExpression *
parse_postfix_expression(const struct Token ***);
static struct ArgumentExpressionList *
parse_argument_expression_list(const struct Token ***);
static struct UnaryExpression *parse_unary_expression(const struct Token ***);
static struct UnaryOperator *parse_unary_operator(const struct Token ***);
static struct CastExpression *parse_cast_expression(const struct Token ***);
static struct MultiplicativeExpression *
parse_multiplicative_expression(const struct Token ***);
static struct AdditiveExpression *
parse_additive_expression(const struct Token ***);
static struct ShiftExpression *parse_shift_expression(const struct Token ***);
static struct RelationalExpression *
parse_relational_expression(const struct Token ***);
static struct EqualityExpression *
parse_equality_expression(const struct Token ***);
static struct ANDExpression *parse_a_n_d_expression(const struct Token ***);
static struct ExclusiveORExpression *
parse_exclusive_o_r_expression(const struct Token ***);
static struct InclusiveORExpression *
parse_inclusive_o_r_expression(const struct Token ***);
static struct LogicalANDExpression *
parse_logical_a_n_d_expression(const struct Token ***);
static struct LogicalORExpression *
parse_logical_o_r_expression(const struct Token ***);
static struct ConditionalExpression *
parse_conditional_expression(const struct Token ***);
static struct AssignmentExpression *
parse_assignment_expression(const struct Token ***);
static struct AssignmentOperator *
parse_assignment_operator(const struct Token ***);
static struct Expression *parse_expression(const struct Token ***);
static struct ConstantExpression *
parse_constant_expression(const struct Token ***);

struct AST *parse(struct Token **tokens) {
  ERROR("Not implemented yet");
}

static void next_token(const struct Token ***const tokens) { ++(*tokens); }

static void restore_position_if(bool cond, const struct Token ***dst,
                                const struct Token **src) {
  if (cond) {
    *dst = src;
  }
}

/*
static bool accept_token(struct Token ***tokens, enum TokenKind kind) {
  if ((**tokens)->kind != kind) {
    return false;
  }

  ++(*tokens);
  return true;
}

static bool expect_token(struct Token ***tokens, enum TokenKind kind) {
  if (!accept_token(tokens, kind)) {
    ERROR("expected: %s, found: %s", token_kind_str((int)(**tokens)->kind),
          token_kind_str((int)kind));
    return false;
  }

  return true;
}
*/

static bool accept_punctuator(const struct Token ***tokens,
                              enum PunctuatorKind kind) {
  if ((**tokens)->kind != TOKEN_PUNCTUATOR ||
      (**tokens)->punctuator->kind != kind) {
    return false;
  }

  ++(*tokens);
  return true;
}

static bool expect_punctuator(const struct Token ***tokens,
                              enum PunctuatorKind kind) {
  if (!accept_punctuator(tokens, kind)) {
    ERROR("I DONT CARE");
    return false;
  }

  return true;
}

static struct PrimaryExpression *
parse_primary_expression(const struct Token ***tokens) {
  struct PrimaryExpression *result = MALLOC(sizeof(struct PrimaryExpression));
  const struct Token **const saved = *tokens;

  switch ((**tokens)->kind) {
  case TOKEN_IDENTIFIER:
    result->kind = PE_IDENTIFIER;
    result->identifier = (**tokens)->identifier;
    next_token(tokens);
    return result;

  case TOKEN_CONSTANT:
    result->kind = PE_CONSTANT;
    result->constant = (**tokens)->constant;
    next_token(tokens);
    return result;

  case TOKEN_STRING_LITERAL:
    result->kind = PE_STRING_LITERAL;
    result->string_literal = (**tokens)->string_literal;
    next_token(tokens);
    return result;

  case TOKEN_PUNCTUATOR:
    if (accept_punctuator(tokens, LEFT_PAREN) &&
        (result->expression = parse_expression(tokens)) &&
        expect_punctuator(tokens, RIGHT_PAREN)) {
      // succeeded
      result->kind = PE_EXPRESSION;
      return result;
    }

    // failed
    break;

  case TOKEN_KEYWORD:
    if (compare_token_keyword(**tokens, GENERIC) &&
        (result->generic_selection = parse_generic_selection(tokens))) {
      // succeeded
      result->kind = PE_GENERIC_SELECTION;
      return result;
    }
    // failed
    break;
  }

  // failed
  *tokens = saved;
  FREE(result);
  return NULL;
}

static struct GenericSelection *
parse_generic_selection(const struct Token ***const tokens) {
  ERROR("unimplemented");
}

static struct GenericAssocList *
parse_generic_assoc_list(const struct Token ***const tokens) {
  ERROR("unimplemented");
}

static struct GenericAssociation *
parse_generic_association(const struct Token ***const tokens) {
  ERROR("unimplemented");
}

static struct PostfixExpression *
parse_postfix_expression(const struct Token ***const tokens) {
  ERROR("unimplemented");
}

static struct ArgumentExpressionList *
parse_argument_expression_list(const struct Token ***const tokens) {
  ERROR("unimplemented");
}

static struct UnaryExpression *
parse_unary_expression(const struct Token ***const tokens) {
  ERROR("unimplemented");
}

static struct UnaryOperator *
parse_unary_operator(const struct Token ***const tokens) {
  ERROR("unimplemented");
}

static struct CastExpression *
parse_cast_expression(const struct Token ***const tokens) {
  ERROR("unimplemented");
}

static struct MultiplicativeExpression *
parse_multiplicative_expression(const struct Token ***const tokens) {
  ERROR("unimplemented");
}

static struct AdditiveExpression *
parse_additive_expression(const struct Token ***const tokens) {
  ERROR("unimplemented");
}

static struct ShiftExpression *
parse_shift_expression(const struct Token ***const tokens) {
  ERROR("unimplemented");
}

static struct RelationalExpression *
parse_relational_expression(const struct Token ***const tokens) {
  ERROR("unimplemented");
}

static struct EqualityExpression *
parse_equality_expression(const struct Token ***const tokens) {
  ERROR("unimplemented");
}

static struct ANDExpression *
parse_a_n_d_expression(const struct Token ***const tokens) {
  ERROR("unimplemented");
}

static struct ExclusiveORExpression *
parse_exclusive_o_r_expression(const struct Token ***const tokens) {
  ERROR("unimplemented");
}

static struct InclusiveORExpression *
parse_inclusive_o_r_expression(const struct Token ***const tokens) {
  ERROR("unimplemented");
}

static struct LogicalANDExpression *
parse_logical_a_n_d_expression(const struct Token ***const tokens) {
  ERROR("unimplemented");
}

static struct LogicalORExpression *
parse_logical_o_r_expression(const struct Token ***const tokens) {
  ERROR("unimplemented");
}

static struct ConditionalExpression *
parse_conditional_expression(const struct Token ***const tokens) {
  ERROR("unimplemented");
}

static struct AssignmentExpression *
parse_assignment_expression(const struct Token ***const tokens) {
  ERROR("unimplemented");
}

static struct AssignmentOperator *
parse_assignment_operator(const struct Token ***const tokens) {
  ERROR("unimplemented");
}

static struct Expression *parse_expression(const struct Token ***const tokens) {
  ERROR("unimplemented");
}

static struct ConstantExpression *
parse_constant_expression(const struct Token ***const tokens) {
  ERROR("unimplemented");
}

// vim: set ft=c ts=2 sw=2 et:
