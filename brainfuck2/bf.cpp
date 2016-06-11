#include <vector>
#include <string>
#include <map>
#include <fstream>

using namespace std;

enum op_type { INC, MOVE, LOOP, PRINT };
struct Op;
typedef vector<Op> Ops;

struct Op {
  op_type op;
  int val;
  Ops loop;
  Op(Ops v) : op(LOOP), loop(v) {}
  Op(op_type _op, int v = 0) : op(_op), val(v) {}
};

class Tape {
  int pos;
  vector<int> tape;

public:
  Tape() {
    pos = 0;
    tape.push_back(0);
  }

  int get() { return tape[pos]; }
  void inc(int x) { tape[pos] += x; }
  void move(int x) { 
    int new_pos = pos + x;
    if (new_pos >= tape.size()) {
      int delta = new_pos - tape.size() + 1;
      for (int i = 0; i < delta; i++) tape.push_back(0);
    }
    if (new_pos >= 0) pos = new_pos;
  }
};

Ops parse(string::iterator *iterator, string::iterator end) {
  Ops res;
  while (*iterator != end) {
    char c = **iterator;
    *iterator += 1;
    switch (c) {      
      case '+': res.push_back(Op(INC, 1)); break;
      case '-': res.push_back(Op(INC, -1)); break;
      case '>': res.push_back(Op(MOVE, 1)); break;
      case '<': res.push_back(Op(MOVE, -1)); break;
      case '.': res.push_back(Op(PRINT)); break;
      case '[': res.push_back(Op(parse(iterator, end))); break;
      case ']': return res;
    }
  }
  return res;
}

void run(Ops &program, Tape &tape) {
  for (Ops::iterator it = program.begin(); it != program.end(); it++) {
    Op &op = *it;
    switch (op.op) {
      case INC: tape.inc(op.val); break;
      case MOVE: tape.move(op.val); break;
      case LOOP: while (tape.get() != 0) run(op.loop, tape); break;
      case PRINT: printf("%c", tape.get()); fflush(stdout); break;
    }
  }
}

string read_file(string filename){
  ifstream textstream(filename.c_str());
  textstream.seekg(0, ios_base::end);
  const int lenght = textstream.tellg();
  textstream.seekg(0);
  string text(lenght, ' ');
  textstream.read(&text[0], lenght);
  textstream.close();
  return text;
}

int main(int argc, char** argv){
  string text = read_file(string(argv[1]));
  Tape tape;
  string::iterator iterator = text.begin();
  Ops program = parse(&iterator, text.end());
  run(program, tape);
  return 0;
}

