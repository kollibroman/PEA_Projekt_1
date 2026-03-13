export module IAtspAlgorithm;

export class IAtspAlgorithm {
public:
    virtual ~IAtspAlgorithm() = default;

    virtual void SolveProblem() const = 0;
};