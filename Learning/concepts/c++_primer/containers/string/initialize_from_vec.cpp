// ==================================================
// EXTRA CODE (append only)
// ==================================================

// Count vowels in a vector<char>
int count_vowels(const std::vector<char>& v) {
    int count = 0;
    for (char c : v) {
        if (c=='a'||c=='e'||c=='i'||c=='o'||c=='u'||
            c=='A'||c=='E'||c=='I'||c=='O'||c=='U')
            ++count;
    }
    return count;
}

// Convert vector<char> to uppercase string
std::string to_upper_string(const std::vector<char>& v) {
    std::string s;
    for (char c : v) {
        if (c >= 'a' && c <= 'z')
            s += (c - 'a' + 'A');
        else
            s += c;
    }
    return s;
}

// Auto-run extra demo (executes before main)
int __ = [](){
    std::cout << "\n--- Extra Vector<char> Demo ---\n";

    std::vector<char> demo = {'C','p','p','!'};

    std::cout << "Demo string: ";
    std::string s(demo.begin(), demo.end());
    std::cout << s << '\n';

    std::cout << "Uppercase: " << to_upper_string(demo) << '\n';
    std::cout << "Vowel count: " << count_vowels(demo) << '\n';

    demo.push_back('?');
    std::cout << "After push_back: ";
    for(char c : demo) std::cout << c;
    std::cout << '\n';

    return 0;
}();
