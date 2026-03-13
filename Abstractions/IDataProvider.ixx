export module IDataProvider;

export class IDataProvider
{
public:
    virtual ~IDataProvider() = 0;

    virtual void GetRequiredData() const = 0;
};