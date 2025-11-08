export module skbase.memory.array;

import std;

#include "../defines.h++"

export namespace Arrays {
    template<typename item_t>
    class Array : public std::vector<item_t> {
    public :
		using std::vector<item_t>::vector;

        Array(std::initializer_list<item_t> init)
            :std::vector<item_t>(init) {}

        Array(std::vector<item_t> values)
            :std::vector<item_t>(values) {}

        Array(item_t value_, std::size_t size) {
            this->reserve(size);
            this->fill(value_);
        }

		Array() {}

        template<typename Acc>
        inline Acc reduce(std::function<Acc(Acc, const item_t&)> reducer, Acc initial) const
            { return accumulate(this->begin(), this->end(), initial, reducer); }

        inline bool contains(item_t val)
            { return find(this->begin(), this->end(), val) != this->end(); }

        inline double average() const
            { return this->empty() ? 0 : static_cast<double>(sum()) / this->size(); }

        inline item_t product() const
            { return Arrays::Array<item_t>::accumulate(this->begin(), this->end(), item_t(1), std::multiplies<item_t>()); }

        inline void foreach(std::function<void(item_t&)> action)
            { for (auto& item : *this) action(item); }

        inline item_t sum() const
            { return accumulate(this->begin(), this->end(), item_t()); }

        inline void reverse()
            { reverse(this->begin(), this->end()); }



        inline void remove(std::size_t index) {
            if (index >= this->size() || this->empty()) return;
            this->erase(this->begin() + index);
        }
        inline item_t removed(std::size_t index) {
            if (index >= this->size() || this->empty()) return 0;
            item_t ret = this->at(index);
            this->erase(this->begin() + index);
            return ret;
        }

        inline void remove(std::vector<std::size_t> indexs) {
            if (this->empty()) return;
            for (std::size_t index: indexs) remove(index);
        }
        inline Array<item_t> removed(std::vector<std::size_t> indexs) {
            if (this->empty()) return Array<item_t>();
            Array<item_t> ret;
            for (std::size_t index: indexs)
                ret += removed(index);
            return ret;
        }

        template<std::size_t index_num = 0>
        item_t removeV(item_t ret) {
            std::size_t index = indexOf<index_num>(ret);
            this->erase(this->begin() + index);
            return ret;
        }

        Array<item_t> removeV(std::vector<item_t> ret, std::vector<int> index_num) {
            for (std::size_t i = 0; i < std::size_t(ret.size()); i++)
                removeV(ret[i], index_num[i]);
            return ret;
        }

        item_t removeFV(item_t ret) {
            std::size_t index = firstOf(ret);
            this->erase(this->begin() + index);
            return ret;
        }

        Array<item_t> removeFV(std::vector<item_t> ret, std::vector<int> index_num) {
            for (std::size_t i = 0; i < std::size_t(ret.size()); i++) removeFV(ret[i], index_num[i]);
            return ret;
        }

        item_t removeLV(item_t ret) {
            std::size_t index = lastOf(ret);
            this->erase(this->begin() + index);
            return ret;
        }

        Array<item_t> removeLV(std::vector<item_t> ret, std::vector<int> index_num) {
            for (std::size_t i = 0; i < std::size_t(ret.size()); i++) removeLV(ret[i], index_num[i]);
            return ret;
        }


        // remove First the first `times` items from the array
        template<std::size_t times=1>
        void removeF() {
            if (this->empty()) return;
            for (std::size_t i=times; i>0 && !this->empty(); --i)
                this->erase(this->begin());
        }

        /* removed First:
            * remove the first `times` items from the array
            * and get them as an other array
        */
        template<std::size_t times=1>
        auto removedF() {
            if (this->empty()) return;
            if constexpr (times == 1) {
                item_t ret= this->at(0);
                this->erase(this->begin());
                return ret;
            } else {
                Array<item_t> ret;
                for (std::size_t i=times; i>0 && !this->empty(); --i) {
                    ret.add(this->at(0));
                    this->erase(this->begin());
                }
                return ret;
            }
        }


        // remove Last  the last `times` items from the array
        template<std::size_t times=1>
        void removeL() {
            if (this->empty()) return;
            for (std::size_t i=times; i>0 && !this->empty(); --i)
                this->erase(this->begin() + this->size() - 1);
        }

        /* removed Last:
            * remove the last `times` items from the array
            * and get them as an other array
        */
        template<std::size_t times=1>
        auto removedL() {
            if (this->empty()) return;
            if constexpr (times == 1) {
                std::size_t len= this->size()-1;
                item_t ret= this->at(len);
                this->erase(this->begin()+len);
                return ret;
            } else {
                Array<item_t> ret;
                for (std::size_t i=times; i>0 && !this->empty(); --i) {
                    std::size_t len= this->size()-1;
                    ret.add(this->at(len));
                    this->erase(this->begin()+len);
                }
                return ret;
            }
        }

        // take this string with out a (first, end) from position selected with inputs
        template<std::size_t length=0, std::size_t i=0>
        inline Array<item_t> without_start(item_t str)
            { return subarray_ii(indexOf<i>(str), this->size()-length); }
        template<std::size_t length=0, std::size_t i=0>
        inline Array<item_t> without_end(item_t str)
            { return subarray_ii(length,              indexOf<i>(str)); }
        inline Array<item_t> without_startF(item_t str) { return subarray_ii(firstOf(str), this->size()); }
        inline Array<item_t> without_startL(item_t str) { return subarray_ii(lastOf(str),  this->size()); }
        inline Array<item_t> without_endF  (item_t str) { return subarray_ii(0,            firstOf(str)); }
        inline Array<item_t> without_endL  (item_t str) { return subarray_ii(0,             lastOf(str)); }

        inline Array<item_t> without_end  () { return subarray_ii(0, this->size()-1); }
        inline Array<item_t> without_start() { return subarray_ii(1, this->size()  ); }

        // default actions
        template<typename custem_type, typename returned = item_t>
        inline void custem_add(
                std::vector<custem_type> item,
                std::function<returned(custem_type)> the_how = [](custem_type arg) {return arg;}
        ) { transform(item.begin(), item.end(), this->begin(), the_how); }

        inline void do_for_all(std::function<void(item_t)> action)
            { for (int i = 0; i < int(this->size());i++) action(this[i]); }

        inline void add(item_t value_)
            { this->push_back(value_); }

        inline void add(std::vector<item_t> values, std::function<item_t(item_t)> trans) {
            this->reserve(this->size() + values.size());
            for (item_t i : values) this->add(trans(i));
        }
        inline void add(std::vector<item_t> values) {
            this->reserve(this->size() + values.size());
            for (item_t i : values) this->add(i);
        }
		inline void add(std::span<item_t> values) {
			for (auto& v : values)
				this->push_back(v);
		}

        inline Array<item_t> End  (std::size_t length_) { return subarray_ii(this->size() - length_, this->size()); }
        inline Array<item_t> Start(std::size_t length_) { return subarray_ii(0, length_); }
        inline item_t last () { return get(this->size() - 1); }
        inline item_t first() { return this[0]; }
        inline item_t& pyget(long int i) {
            if (i < 0)
                return this->pyget((long int)this->size() + i);
            if (i > (long int)this->size())
                return this->pyget(i - (long int)this->size());
            return this->at((std::size_t)i);
        }

        // take a cut
        Array<item_t> subarray_ii(int start, int end) {
            Array<item_t> ret = Arrays::Array<item_t>();
            for (int i = start; i < end + 1 && i < this->size();i++)
                ret.add(this[i]);
            return ret;
        }
        Array<item_t> subarray_vivi(item_t start, item_t end, int numstart, int numend) {
            Array<item_t> ret = Arrays::Array<item_t>();
            std::size_t S = (std::size_t)indexOf(start, numstart), E = (std::size_t)indexOf(end, numend);
            for (std::size_t i = S; i < E + 1 && i < this->size();i++)
                ret.add(this[i]);
            return ret;
        }
        Array<item_t> subarray_iv(int start, item_t end)  {
            Array<item_t> ret = Arrays::Array<item_t>();
            for (int i = start; i < firstof(end) && i < this->size();i++)
                ret.add(this[i]);
            return ret;
        }
        Array<item_t> subarray_vi(item_t start, int end) {
            Array<item_t> ret = Arrays::Array<item_t>();
            for (int i = firstof(start); i < end + 1 && i < this->size();i++)
                ret.add(this[i]);
            return ret;
        }
        Array<item_t> subarray(item_t start, item_t end) {
            Array<item_t> ret = Arrays::Array<item_t>();
            for (int i = firstof(start); i < firstof(end) && i < this->size();i++)
                ret.add(this[i]);
            return ret;
        }
        Array<item_t> subarrayL_iv(int start, item_t end) {
            Array<item_t> ret = Arrays::Array<item_t>();
            for (int i = start; i < lastof(end) && i < this->size();i++)
                ret.add(this[i]);
            return ret;
        }
        Array<item_t> subarrayL_vi(item_t start, int end) {
            Array<item_t> ret = Arrays::Array<item_t>();
            for (int i = lastof(start); i < end + 1 && i < this->size();i++)
                ret.add(this[i]);
            return ret;
        }
        Array<item_t> subarrayLL(item_t start, item_t end) {
            Array<item_t> ret = Arrays::Array<item_t>();
            for (int i = lastof(start); i < lastof(end) && i < this->size();i++)
                ret.add(this[i]);
            return ret;
        }
        Array<item_t> subarrayLF(item_t start, item_t end) {
            Array<item_t> ret = Arrays::Array<item_t>();
            for (int i = lastof(start); i < firstof(end) && i < this->size();i++)

                ret.add(this[i]);
            return ret;
        }
        Array<item_t> subarrayFL(item_t start, item_t end) {
            Array<item_t> ret = Arrays::Array<item_t>();
            for (int i = firstof(start); i < lastof(end) && i < this->size();i++)
                ret.add(this[i]);
            return ret;
        }
        Array<Array<item_t>> split(std::size_t chunk_size) const {
            Array<Array<item_t>> result;
            for (std::size_t i = 0; i < this->size(); i += chunk_size) {
                Array<item_t> chunk;
                for (std::size_t j = i; j < min(i + chunk_size, this->size()); ++j)
                    chunk.add(*this[j]);
                result.add(chunk);
            } return result;
        }

        Array<item_t> slice(std::size_t start, std::size_t end) const {
            end = std::min(end, this->size());
            start = std::min(start, end);
            return (Arrays::Array<item_t>)std::vector<item_t>(this->begin() + start, this->begin() + end);
        }

        Array<Array<item_t>> split(item_t value_) {
            Array<Array<item_t>> ret, array0;
            for (int i = 0; i < int(this->size()); i++)
                if (this[i] == value_) {
                    ret.add(array0);
                    array0.clear();
                } else
                    array0.add(this[i]);
            return ret;
        }

        // find
        inline std::size_t numberOf(item_t value_) { return split(value_).size() - 1; }
        inline std::size_t firstOf (item_t value_) { return indexOf<0>(value_); }
        Array<std::size_t> indexsOf(item_t value_) {
            Array<item_t> ret = *this;
            Array<std::size_t> indexs = Array<std::size_t>();
            for (std::size_t i = 0;; i++) {
                auto it = find(this->begin(), this->end(), value_);
                std::size_t dlti = it != this->end() ? distance(this->begin(), it) : -1;
                if (dlti == -1) break;
                else {
                    indexs.add(std::size_t(ret.indexOf(dlti)));
                    ret.remove(indexs.last());
                }
            } return indexs;
        }
        template<std::size_t index_num=0>
        std::size_t indexOf(const item_t& value_) {
            if constexpr (index_num > 0) {
                std::size_t num=0;
                for (std::size_t i=0, size=this->size(); i<size; i++) {
                    if (this->at(i) == value_)
                        num++;
                    if (num == index_num)
                        return i;
                }
            } else {
                for (std::size_t i=0, size=this->size(); i<size; i++)
                  if (this->at(i) == value_)
                      return i;
                std:: cerr << "\e[1;31mError\e[0m: the value that you give is no found in this array\n"
                           << "\tthat 'indexOf<std::size_t index_num="<<index_num<<">(const item_t& value_) function'\n"
                           << "\twill return '-1' that may make same errors in use\n";
                return -1;
            }
        }
        std::size_t lastOf(const item_t& value_) {
            for (std::size_t i=this->size(); i > 0; i--)
                if (this->at(i) == value_)
                    return i;
            return this->at(-1);
        }

        // additions
        Array<item_t> filter(std::function<bool(const item_t&)> predicate) const {
            Array<item_t> result;
            copy_if(this->begin(), this->end(), back_inserter(result.data), predicate);
            return result;
        }
        template<typename U> Array<U> map(std::function<U(const item_t&)> mapper) const {
            Array<U> result;
            transform(this->begin(), this->end(), back_inserter(result.data), mapper);
            return result;
        }

        // sort
        void sort(bool ascending = true) { ascending ? sort(this->begin(), this->end()) : std::sort(this->begin(), this->end(), std::greater<item_t>()); }
        inline void sort_by(std::function<bool(const item_t&, const item_t&)> comparator) { std::sort(this->begin(), this->end(), comparator); }

        // unique & intersection
        Array<item_t> intersection(const Array<item_t>& other) const {
            Array<item_t> result;
            set_intersection(this->begin(), this->end(),
                other.begin(), other.end(),
                back_inserter(result.data)
            ); return result;
        }
        inline Array<item_t> union_with(const Array<item_t>& other) const {
            Array<item_t> result;
            this->set_union(this->begin(), this->end(),
                other.begin(), other.end(),
                back_inserter(result.data)
            ); return result;
        }
        Array<item_t> unique() const {
            Array<item_t> result = *this;
            sort(result.begin(), result.end());
            auto last = unique(result.begin(), result.end());
            result.erase(last, result.end());
            return result;
        }

        bool containsAll(std::vector<item_t> content) {
            bool ret = true;
            for (item_t i : content) if (!this->contains(i)) {
                ret = false;
                break;
            } return ret;
        }
        bool containsAll(const item_t* content) {
            bool ret = true;
            for (item_t i : content) if (!this->contains(i)) {
                ret = false;
                break;
            } return ret;
        }

        bool containsOneOf(std::vector<item_t> content) {
            bool ret = false;
            for (item_t i : content) if (this->contains(i)) {
                ret = true;
                break;
            } return ret;
        }
        bool containsOneOf(const item_t* content) {
            bool ret = false;
            for (item_t i : content) if (this->contains(i)) {
                ret = true;
                break;
            } return ret;
        }

        // operators
        inline Array<item_t> operator ()(int start, int end) { return subarray_ii(start, end); }

        inline Array<Array<item_t>> operator /(item_t value_) { return split(value_); }

        inline void operator = (std::vector<item_t> value_) { for (item_t var : value_) this->add(var); }
        inline void operator +=(std::vector<item_t> value_) { add(value_); }
        inline void operator +=(item_t* value_) { add(value_); }
        inline void operator +=(item_t value_) { add(value_); }
        inline Array<item_t> operator +(std::vector<item_t> value_) {
            Array<item_t> ret = *this;
            ret.add(value_);
            return ret;
        }
        inline Array<item_t> operator +(item_t* value_) {
            Array<item_t> ret = *this;
            ret.add(value_);
            return ret;
        }
        inline Array<item_t> operator +(item_t value_) {
            Array<item_t> ret = *this;
            ret.add(value_);
            return ret;
        }

        inline item_t operator -(std::size_t index) { return remove(index); }
        inline item_t operator --() {
            item_t last = this->last();
            this->pop_back();
            return last;
        }
    };
}
