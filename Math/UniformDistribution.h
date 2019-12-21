#ifndef _Math_UniformDistribution_h_
#define _Math_UniformDistribution_h_

template <typename T>
class UniformDistribution {
public:
	UniformDistribution(T a = 0.0, T b = 1.0) : m_a(a), m_b(b) {

	}

	void reset() { m_a = 0.0; m_b = 1.0; }

	template <class Gen>
	T operator() (Gen &g) {
		double scale = (m_b - m_a) / (T(g.max() - g.min()) + T(1));
		return g() - g.min() * scale + m_a;
	}

protected:
	T m_a;
	T m_b;
};

#endif
