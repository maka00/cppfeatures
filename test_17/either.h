//
// Created by mk on 18.04.21.
//

#pragma once
#include <variant>

template< typename L, typename R>
struct Either {
    std::variant<L, R> value;
    explicit Either(L left) : value(std::move(left)) { };
    explicit Either(R right) : value(std::move(right)) { };
    Either() = delete;
    ~Either() = default;
    Either(Either<L,R>&& other) noexcept : value(std::move(other.value)) { };
    Either<L,R> operator() (const Either<L,R>& other ) { value = other.value; };
    Either(const Either<L,R>& other) : value(other.value) { };
    L& get_left() {
        return std::get<L>(value);
    }

    R& get_right() {
        return std::get<R>(value);
    }

    constexpr bool has_left() const {
        return std::holds_alternative<L>(value);
    }
    constexpr bool has_right() const {
        return std::holds_alternative<R>(value);
    }
};
