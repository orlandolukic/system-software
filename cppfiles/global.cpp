#include <regex>

std::regex REGEX_DIRECTIVE("\\s*\\.[a-z]+\\s*([a-zA-Z\"% 0-9,\\W\\\\_]*|(0|-?[1-9][0-9]*)(\\s*,\\s*(0|-?[1-9][0-9]*|[a-zA-Z_][a-zA-Z_0-9]*))*)");
std::regex REGEX_DIRECTIVE_NAME("\\.[a-z]+");
std::regex REGEX_COMMENT("\\s*#[a-zA-Z0-9_\\W]*");
std::regex REGEX_NEGATIVE_NUMBER("-[1-9][0-9]*");
std::regex REGEX_NUMBER("[1-9][0-9]*");
std::regex REGEX_FIND_ONLY_NUMBER("^(0|-?[1-9][0-9]*)");
std::regex REGEX_FIND_ONLY_LABEL("[a-zA-Z_][a-zA-Z_0-9]*");

std::regex REGEX_LABEL("[a-zA-Z_][a-zA-Z_0-9]*");
std::regex REGEX_ALONE_LABEL("\\s*[a-zA-Z_][a-zA-Z_0-9]*:");
//std::regex REGEX_INSTRUCTION_CHECK("\\s*([a-z]{2,4}|[A-Z]{2,4})[bw]? ?(sp|\\[sp\\]|r[0-7]|\\[r[0-7]\\]|(sp|r[0-7])\\[-?[1-9][0-9]*\\]|\\$[a-zA-Z_][a-zA-Z_0-9]|r[0-7]\\[[a-zA-Z_][a-zA-Z_0-9]\\]|\\*0x[1-9a-fA-F][0-9a-fA-F]{0,3}|r[0-7]\\[[a-zA-Z0-9_][a-zA-Z_0-9]*\\])?,?\\s*(sp|\\[sp\\]|r[0-7]|\\[r[0-7]\\]|r[0-7]|(sp|r[0-7])\\[-?[1-9][0-9]*\\]|\\*?0x[1-9a-fA-F][0-9a-fA-F]{0,3}|\\$?[a-zA-Z0-9_][a-zA-Z_0-9]*|(-?[1-9][0-9]*|0))?");
std::regex REGEX_INSTRUCTION_OP0("\\s*(halt|i?ret)\\s*");
std::regex REGEX_INSTRUCTION_OP1("\\s*([a-z]{2,4}|[A-Z]{2,4})(b|w)? ((0|-?[1-9][0-9]*)|sp(h|l)?|\\[sp\\]|r[0-7](h|l)?|\\[r[0-7]\\]|(sp|r[0-7])\\[-?[1-9][0-9]*\\]|(sp|r[0-7])\\[[a-zA-Z_][a-zA-Z_0-9]*\\]|(\\$|&)?[a-zA-Z_][a-zA-Z_0-9]*|r[0-7]\\[[a-zA-Z_][a-zA-Z_0-9]*\\]|\\*0x[1-9a-fA-F][0-9a-fA-F]{0,3}|r[0-7]\\[[a-zA-Z0-9_][a-zA-Z_0-9]*\\])");
std::regex REGEX_INSTRUCTION_OP2("\\s*([a-z]{2,4}|[A-Z]{2,4})(b|w)? (sp(h|l)?|\\[sp\\]|r[0-7](h|l)?|\\[r[0-7]\\]|(sp|r[0-7])\\[-?[1-9][0-9]*\\]|(sp|r[0-7])[a-zA-Z_][a-zA-Z_0-9]*|(sp|r[0-7])\\[[a-zA-Z_][a-zA-Z_0-9]*\\]|r[0-7]\\[[a-zA-Z_][a-zA-Z_0-9]\\]|\\*0x[0-9a-fA-F]{1,4}|r[0-7]\\[[a-zA-Z0-9_][a-zA-Z_0-9]*\\]|\\$?[a-zA-Z_][a-zA-Z_0-9]*),\\s*(sp(h|l)?|\\[sp\\]|r[0-7](h|l)?|\\[r[0-7]\\]|(sp|r[0-7])\\[-?[1-9][0-9]*\\]|(sp|r[0-7])\\[[a-zA-Z_][a-zA-Z_0-9]*\\]|\\*?0x[0-9a-fA-F]{1,4}|(\\$|&)?[a-zA-Z0-9_][a-zA-Z_0-9]*|(-?[1-9][0-9]*|0))");
std::regex REGEX_INSTRUCTION_NAME("^\\s*([a-z]{2,4}|[A-Z]{2,4})");
std::regex REGEX_LABEL_DIRECTIVE("\\s*[a-zA-Z_][a-zA-Z_0-9]*:\\s*\\.[a-z]+\\s*([a-zA-Z\"% 0-9,\\W\\\\]*|(0|-?[1-9][0-9]*)(\\s*,\\s*(0|-?[1-9][0-9]*||[a-zA-Z_][a-zA-Z_0-9]*))*)");
std::regex REGEX_LABEL_INSTRUCTION("\\s*[a-zA-Z0-9_][a-zA-Z_0-9]*:\\s*([a-z]{2,4}|[A-Z]{2,4})[bw]? ?(sp|r[0-7]|(sp|r[0-7])\\[-?[1-9][0-9]*\\]|\\$[a-zA-Z_][a-zA-Z_0-9]|r[0-7]\\[[a-zA-Z_][a-zA-Z_0-9]\\]|\\*0x[1-9a-fA-F][0-9a-fA-F]{0,3}|r[0-7]\\[[a-zA-Z_][a-zA-Z_0-9]*\\]|\\&[a-zA-Z_][a-zA-Z_0-9]*)?,?\\s*(sp|r[0-7]|(sp|r[0-7])\\[-?[1-9][0-9]*\\]|\\*?0x[1-9a-fA-F][0-9a-fA-F]{0,3}|(\\$|&)?[a-zA-Z_][a-zA-Z_0-9]*|(-?[1-9][0-9]*|0))?");
std::regex REGEX_OPERAND_1("((0|-?[1-9][0-9]*)|sp|\\[sp\\]|r[0-7]|\\[r[0-7]\\]|(sp|r[0-7])\\[-?[1-9][0-9]*\\]|\\$[a-zA-Z_][a-zA-Z_0-9]|[a-zA-Z0-9_][a-zA-Z_0-9]*|r[0-7]\\[[a-zA-Z_][a-zA-Z_0-9]\\]|\\*0x[1-9a-fA-F][0-9a-fA-F]{0,3}|r[0-7]\\[[a-zA-Z0-9_][a-zA-Z_0-9]*\\]|&[a-zA-Z_][a-zA-Z_0-9]*)");
std::regex REGEX_OPERAND_2("(sp|\\[sp\\]|r[0-7]|\\[r[0-7]\\]|(sp|r[0-7])\\[-?[1-9][0-9]*\\]|\\*?0x[1-9a-fA-F][0-9a-fA-F]{0,3}|\\$?[a-zA-Z0-9_][a-zA-Z_0-9]*|(-?[1-9][0-9]*|0))");
std::regex REGEX_EMPTY_LINE("^\\s*$");
std::regex REGEX_REMOVE_BLANKS_END("\\s*$");

std::regex REGEX_BYTE("(0|-?[1-9][0-9]*)(\\s*,\\s*(0|-?[1-9][0-9]*))*");
std::regex REGEX_WORD("(0|-?[1-9][0-9]*|[a-zA-Z_][a-zA-Z_0-9]*)(\\s*,\\s*(0|-?[1-9][0-9]*|[a-zA-Z_][a-zA-Z_0-9]*))*");






