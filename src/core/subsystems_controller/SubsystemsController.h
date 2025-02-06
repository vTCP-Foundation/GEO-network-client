#ifndef GEO_NETWORK_CLIENT_SUBSYSTEMSCONTROLLER_H
#define GEO_NETWORK_CLIENT_SUBSYSTEMSCONTROLLER_H

#include "../common/Types.h"
#include "../contractors/addresses/BaseAddress.h"
#include "../logger/Logger.h"

#include <chrono>
#include <thread>

class SubsystemsController
{

public:
    SubsystemsController(
        Logger &log);

public:
    void setFlags(size_t flags);

    void setForbiddenNodeAddress(
        BaseAddress::Shared nodeAddress);

    void setForbiddenAmount(
        const TrustLineAmount &forbiddenAmount);

    bool isNetworkOn();

    bool isRunCycleClosingTransactions() const;

    bool isRunPaymentTransactions() const;

    bool isRunTrustLineTransactions() const;

    void turnOffNetwork();

    void turnOnNetwork();

    void testForbidSendMessageToReceiverOnReservationStage(
        uint32_t countForbiddenMessages = 1);

    void testForbidSendMessageToCoordinatorOnReservationStage(
        BaseAddress::Shared previousNodeAddress,
        const TrustLineAmount &forbiddenAmount,
        uint32_t countForbiddenMessages = 1);

    void testForbidSendRequestToIntNodeOnReservationStage(
        BaseAddress::Shared receiverMessageNodeAddress,
        const TrustLineAmount &forbiddenAmount,
        uint32_t countForbiddenMessages = 1);

    void testForbidSendResponseToIntNodeOnReservationStage(
        BaseAddress::Shared receiverMessageNodeAddress,
        const TrustLineAmount &forbiddenAmount,
        uint32_t countForbiddenMessages = 1);

    void testForbidSendMessageWithFinalPathConfiguration(
        uint32_t countForbiddenMessages = 1);

    void testForbidSendMessageOnFinalAmountClarificationStage(
        uint32_t countForbiddenMessages = 1);

    void testForbidSendMessageOnVoteStage(
        uint32_t countForbiddenMessages = 1);

    void testForbidSendMessageOnRecoveryStage(
        uint32_t countForbiddenMessages = 1);

    void testForbidSendMessageOnVoteConsistencyStage(
        uint32_t countForbiddenMessages = 1);

    void testThrowExceptionOnPreviousNeighborRequestProcessingStage();

    void testThrowExceptionOnCoordinatorRequestProcessingStage();

    void testThrowExceptionOnNextNeighborResponseProcessingStage();

    void testThrowExceptionOnVoteStage();

    void testThrowExceptionOnVoteConsistencyStage();

    void testThrowExceptionOnCoordinatorAfterApproveBeforeSendMessage();

    void testTerminateProcessOnPreviousNeighborRequestProcessingStage();

    void testTerminateProcessOnCoordinatorRequestProcessingStage();

    void testTerminateProcessOnNextNeighborResponseProcessingStage();

    void testTerminateProcessOnVoteStage();

    void testTerminateProcessOnVoteConsistencyStage();

    void testTerminateProcessOnCoordinatorAfterApproveBeforeSendMessage();

    void testSleepOnPreviousNeighborRequestProcessingStage(
        uint32_t millisecondsDelay);

    void testSleepOnCoordinatorRequestProcessingStage(
        uint32_t millisecondsDelay);

    void testSleepOnNextNeighborResponseProcessingStage(
        uint32_t millisecondsDelay);

    void testSleepOnFinalAmountClarificationStage(
        uint32_t millisecondsDelay);

    void testSleepOnOnVoteStage(
        uint32_t millisecondsDelay);

    void testSleepOnVoteConsistencyStage(
        uint32_t millisecondsDelay);

protected:
    LoggerStream info() const;

    LoggerStream debug() const;

    LoggerStream warning() const;

    const string logHeader() const;

private:
    bool mIsNetworkOn;
    uint32_t mCountForbiddenMessages;

    bool mIsRunCycleClosingTransactions;
    bool mIsRunPaymentTransactions;
    bool mIsRunTrustLineTransactions;

    bool mForbidSendMessageToReceiverOnReservationStage;
    bool mForbidSendMessageToCoordinatorOnReservationStage;
    bool mForbidSendRequestToIntNodeOnReservationStage;
    bool mForbidSendResponseToIntNodeOnReservationStage;
    bool mForbidSendMessageWithFinalPathConfiguration;
    bool mForbidSendMessageOnFinalAmountClarificationStage;
    bool mForbidSendMessageOnVoteStage;
    bool mForbidSendMessageOnVoteConsistencyStage;
    bool mForbidSendMessageOnRecoveryStage;

    bool mThrowExceptionOnPreviousNeighborRequestProcessingStage;
    bool mThrowExceptionOnCoordinatorRequestProcessingStage;
    bool mThrowExceptionOnNextNeighborResponseProcessingStage;
    bool mThrowExceptionOnVoteStage;
    bool mThrowExceptionOnVoteConsistencyStage;
    bool mThrowExceptionOnCoordinatorAfterApproveBeforeSendMessage;

    bool mTerminateProcessOnPreviousNeighborRequestProcessingStage;
    bool mTerminateProcessOnCoordinatorRequestProcessingStage;
    bool mTerminateProcessOnNextNeighborResponseProcessingStage;
    bool mTerminateProcessOnVoteStage;
    bool mTerminateProcessOnVoteConsistencyStage;
    bool mTerminateProcessOnCoordinatorAfterApproveBeforeSendMessage;

    bool mSleepOnPreviousNeighborRequestProcessingStage;
    bool mSleepOnCoordinatorRequestProcessingStage;
    bool mSleepOnNextNeighborResponseProcessingStage;
    bool mSleepOnFinalAmountClarificationStage;
    bool mSleepOnVoteStage;
    bool mSleepOnVoteConsistencyStage;

    BaseAddress::Shared mForbiddenNodeAddress;
    TrustLineAmount mForbiddenAmount;

    Logger &mLog;
};


#endif //GEO_NETWORK_CLIENT_SUBSYSTEMSCONTROLLER_H
