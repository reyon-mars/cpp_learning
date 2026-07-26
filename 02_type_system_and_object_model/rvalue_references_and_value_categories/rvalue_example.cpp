#include <algorithm>
#include <compare>
#include <functional>
#include <iostream>
#include <type_traits>
#include <utility>
#include <vector>

class Widget
{
public:
	int data;
	explicit Widget(int d) : data{d}
	{
		std::cout << "Constructor(" << data << ")\n";
	}
	Widget(const Widget& other) : data{other.data}
	{
		std::cout << "Copy constructor(" << data << ")\n";
	}
	Widget(Widget&& other) noexcept : data{std::exchange(other.data, 0)}
	{
		std::cout << "Move constructor(" << data << ")\n";
	}
	Widget& operator=(Widget other) noexcept
	{
		std::cout << "Assignment (unified)\n";
		std::swap(data, other.data);
		return *this;
	}
	~Widget()
	{
		std::cout << "Destructor(data=" << data << ")\n";
	}
	void reset() noexcept
	{
		data = 0;
	}
	[[nodiscard]] int value() const noexcept
	{
		return data;
	}
	void print() const
	{
		std::cout << "Widget data = " << data << "\n";
	}
	[[nodiscard]] auto operator<=>(const Widget& other) const noexcept
	{
		return data <=> other.data;
	}
	[[nodiscard]] bool operator==(const Widget& other) const noexcept
	{
		return data == other.data;
	}
	friend std::ostream& operator<<(std::ostream& os, const Widget& w)
	{
		return os << "Widget(" << w.data << ")";
	}
	friend void swap(Widget& a, Widget& b) noexcept
	{
		using std::swap;
		swap(a.data, b.data);
	}
};

static_assert(std::is_copy_constructible_v<Widget>);
static_assert(std::is_nothrow_move_constructible_v<Widget>);
static_assert(std::is_nothrow_swappable_v<Widget>);

void consume(Widget w)
{
	std::cout << "Consuming " << w << "\n";
}

void detect(Widget& w)
{
	std::cout << "Lvalue detected: " << w << "\n";
}
void detect(Widget&& w)
{
	std::cout << "Rvalue detected: " << w << "\n";
}

[[nodiscard]] Widget createWidget(int val)
{
	return Widget{val};
}

template <typename T>
void forwardToDetect(T&& arg)
{
	detect(std::forward<T>(arg));
}

template <typename T>
void test_forwarding(T&& arg)
{
	std::cout << "std::move    → ";
	detect(std::move(arg));
	std::cout << "std::forward → ";
	detect(std::forward<T>(arg));
}

[[nodiscard]] Widget chain_move(Widget w)
{
	return w;
}

template <typename... Args>
[[nodiscard]] Widget make_widget(Args&&... args)
{
	return Widget{std::forward<Args>(args)...};
}

[[nodiscard]] Widget mandatory_elision_return(int val)
{
	return Widget{val};
}

[[nodiscard]] Widget nrvo_candidate_return(int val)
{
	Widget local{val};
	return local;
}

[[nodiscard]] Widget forced_move_return(int val)
{
	Widget local{val};
	return std::move(local);
}

void temporary_demo()
{
	std::cout << "\n--- Temporary Demo ---\n";
	Widget temp = createWidget(99);
	temp.print();
}

void self_assignment_demo()
{
	std::cout << "\n--- Self-Assignment Demo ---\n";
	Widget w{123};
	w = w;
	w.print();
}

void vector_demo()
{
	std::cout << "\n--- Vector Demo ---\n";
	std::vector<Widget> widgets;
	widgets.reserve(3);
	for (int val : {1, 2, 3})
	{
		widgets.emplace_back(val);
	}
	std::cout << "Vector size: " << widgets.size() << "\n";
	std::for_each(widgets.cbegin(),
				  widgets.cend(),
				  [](const Widget& w)
				  {
					  w.print();
				  });

	std::cout << "Sorting descending by value:\n";
	std::ranges::sort(widgets, std::greater{});
	for (const auto& w : widgets)
		w.print();

	std::cout << "Finding Widget(2): ";
	if (const auto it = std::ranges::find(widgets, Widget{2}); it != widgets.end())
		std::cout << "found " << *it << "\n";
	else
		std::cout << "not found\n";
}

void vector_growth_move_demo()
{
	std::cout << "\n--- Vector Growth (no reserve, triggers reallocation moves) ---\n";
	std::vector<Widget> widgets;
	for (int val : {1, 2, 3, 4})
	{
		std::cout << "pushing " << val << " (capacity was " << widgets.capacity() << ")\n";
		widgets.push_back(Widget{val});
	}
	std::cout << "final capacity=" << widgets.capacity() << ", size=" << widgets.size() << "\n";
}

void elision_vs_nrvo_vs_forced_move_demo()
{
	std::cout << "\n--- Mandatory elision vs NRVO vs forced-move pessimization ---\n";
	std::cout << "mandatory_elision_return (prvalue, guaranteed no copy/move):\n";
	Widget e = mandatory_elision_return(1);
	std::cout << "e=" << e << "\n";

	std::cout << "\nnrvo_candidate_return (named local, compiler-optional elision):\n";
	Widget n = nrvo_candidate_return(2);
	std::cout << "n=" << n << "\n";

	std::cout << "\nforced_move_return (return std::move(local), defeats NRVO, forces a move):\n";
	Widget f = forced_move_return(3);
	std::cout << "f=" << f << "\n";
}

void comparison_demo()
{
	std::cout << "\n--- operator<=> / operator== ---\n";
	const Widget a{10};
	const Widget b{20};
	std::cout << std::boolalpha << "a == a: " << (a == a) << "\n"
			  << "a == b: " << (a == b) << "\n"
			  << "a < b:  " << (a < b) << "\n"
			  << "a > b:  " << (a > b) << "\n";
}

void swap_demo()
{
	std::cout << "\n--- Swap Demo ---\n";
	Widget a{500};
	Widget b{600};
	std::cout << "Before: " << a << ", " << b << "\n";
	swap(a, b);
	std::cout << "After:  " << a << ", " << b << "\n";
}

void by_value_demo(Widget w)
{
	std::cout << "Pass-by-value: " << w << "\n";
}

int main()
{
	std::cout << "--- Basic Construction & Copy/Move ---\n";
	Widget w1{10};
	Widget w2 = w1;
	Widget w3 = std::move(w1);
	Widget w4{20};
	w4 = w2;
	w4 = std::move(w3);
	consume(std::move(w4));

	std::cout << "\n--- Value Category Detection ---\n";
	Widget w5{50};
	detect(w5);
	detect(std::move(w5));
	detect(createWidget(60));

	std::cout << "\n--- Perfect Forwarding ---\n";
	forwardToDetect(w2);
	forwardToDetect(createWidget(70));

	std::cout << "\n--- Forwarding Test ---\n";
	test_forwarding(w2);
	test_forwarding(createWidget(80));

	temporary_demo();

	std::cout << "\n--- Chain Move ---\n";
	Widget w6 = chain_move(createWidget(200));
	std::cout << "w6: " << w6 << "\n";

	self_assignment_demo();

	std::cout << "\n--- Emplace-Style Construction ---\n";
	Widget w7 = make_widget(300);
	std::cout << "w7: " << w7 << "\n";

	std::cout << "\n--- Reset Demo ---\n";
	w7.reset();
	w7.print();

	std::cout << "\n--- Pass By Value Demo ---\n";
	by_value_demo(w2);

	vector_demo();
	vector_growth_move_demo();
	elision_vs_nrvo_vs_forced_move_demo();
	comparison_demo();
	swap_demo();
	return 0;
}
