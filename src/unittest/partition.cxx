// Copyright (c) 2013 by Bjoern Andres.
// 
// This software was developed by Bjoern Andres.
// Enquiries shall be directed to bjoern@andres.sc.
//
// All advertising materials mentioning features or use of this software must
// display the following acknowledgement: ``This product includes 
// andres::Partition developed by Bjoern Andres. Please direct enquiries 
// concerning andres::Partition to bjoern@andres.sc''.
//
// Redistribution and use in source and binary forms, with or without 
// modification, are permitted provided that the following conditions are met:
//
// - Redistributions of source code must retain the above copyright notice,
//   this list of conditions and the following disclaimer.
// - Redistributions in binary form must reproduce the above copyright notice, 
//   this list of conditions and the following disclaimer in the documentation
//   and/or other materials provided with the distribution.
// - All advertising materials mentioning features or use of this software must 
//   display the following acknowledgement: ``This product includes 
//   andres::Partition developed by Bjoern Andres. Please direct enquiries 
//   concerning andres::Partition to bjoern@andres.sc''.
// - The name of the author must not be used to endorse or promote products 
//   derived from this software without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR IMPLIED 
// WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF 
// MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO 
// EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
// SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
// PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; 
// OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, 
// WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR 
// OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF 
// ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
//
#include <vector>
#include <map>
#include <stdexcept>

#include "andres/partition.hxx"

inline void test(const bool& x) { 
    if(!x) throw std::logic_error("test failed."); 
}

template<class A, class B>
inline void testEqual(const A& a, const B& b) { 
    if(!(a == static_cast<A>(b))) throw std::logic_error("test failed."); 
}

class PartitionTest {
public:
	PartitionTest() {
		p_ = andres::Partition<size_t>(4);
	}

	void constructionTest()	{
		testEqual(emptyP_.numberOfElements(), 0);
		testEqual(emptyP_.numberOfSets(), 0);
		testEqual(p_.numberOfElements(), 4);
		testEqual(p_.numberOfSets(), 4);
	}

    void assignmentTest() {
        p_.merge(0, 1);
        p_.merge(2, 3);
        p_.assign(3);
        testEqual(p_.numberOfElements(), 3);
        testEqual(p_.numberOfSets(), 3);
        testEqual(p_.find(0), 0);
        testEqual(p_.find(1), 1);
        testEqual(p_.find(2), 2);
    }

	void mergeFindTest() {
		// initial finds
		for(size_t j=0; j<4; ++j) {
			testEqual(p_.find(j), j);
		}

		// one merge
		p_.merge(2, 3);
		testEqual(p_.find(0), 0);
		testEqual(p_.find(1), 1);
		testEqual(p_.find(2), p_.find(3));
		test(p_.find(0) != p_.find(2));
		test(p_.find(1) != p_.find(2));
		testEqual(p_.numberOfElements(), 4);
		testEqual(p_.numberOfSets(), 3);
		
		// second merge
		p_.merge(2, 0);
		test(p_.find(0) != p_.find(1));
		test(p_.find(0) == p_.find(2));
		test(p_.find(2) == p_.find(3));
		testEqual(p_.numberOfElements(), 4);
		testEqual(p_.numberOfSets(), 2);
	}

	void insertTest() {
		p_.merge(2, 3);
		p_.insert(2);
		testEqual(p_.numberOfElements(), 6);
		testEqual(p_.numberOfSets(), 5);
		testEqual(p_.find(0), 0);
		testEqual(p_.find(1), 1);
		testEqual(p_.find(2), p_.find(3));
		test(p_.find(2) == 2 || p_.find(2) == 3);
		testEqual(p_.find(4), 4);
		testEqual(p_.find(5), 5);
	}

	void representativesTest() {
		std::vector<size_t> r(size_t(p_.numberOfSets()));
		p_.representatives(r.begin());
		for(size_t j=0; j<4; ++j) {
			testEqual(r[j], j);
		}

		p_.merge(1, 2);
		std::vector<size_t> s(size_t(p_.numberOfSets()));
		p_.representatives(s.begin());
		testEqual(s[0], 0);
		test(s[1] == 1 || s[1] == 2);
		testEqual(s[2], 3);
	}

	void representativeLabelingTest() {
		std::map<size_t, size_t> m;
		m[7] = 15;
		p_.representativeLabeling(m);
		testEqual(m.size(), 4);
		for(size_t j=0; j<4; ++j) {
			test(m.find(j) != m.end());
			testEqual(m[j], j);
		}

		p_.merge(1, 2);
		std::vector<size_t> rep(p_.numberOfSets());
		p_.representatives(rep.begin());
		p_.representativeLabeling(m);
		testEqual(m.size(), 3);
		for(size_t j=0; j<p_.numberOfSets(); ++j) {
			size_t r = rep[j];
			test(m.find(r) != m.end());
			testEqual(m[r], j);
		}		
	}

	void elementLabelingTest() {
		std::vector<size_t> e(p_.numberOfElements());
		p_.elementLabeling(e.begin());
		for(size_t j=0; j<4; ++j) {
			testEqual(e[j], j);
		}

		p_.merge(1, 2);
		std::vector<size_t> f(p_.numberOfElements());
		p_.elementLabeling(f.begin());
		testEqual(f[0], 0);
		testEqual(f[1], 1);
		testEqual(f[2], 1);
		testEqual(f[3], 2);
	}

private:
    andres::Partition<size_t> p_, emptyP_;
};

int main() {
    { PartitionTest t; t.constructionTest(); }
    { PartitionTest t; t.assignmentTest(); }
    { PartitionTest t; t.insertTest(); }
    { PartitionTest t; t.mergeFindTest(); }
    { PartitionTest t; t.elementLabelingTest(); }
    { PartitionTest t; t.representativesTest(); }
    { PartitionTest t; t.representativeLabelingTest(); }
    return 0;
}
