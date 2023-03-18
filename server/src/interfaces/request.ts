import { Request } from "express";

interface RequestWithLockId extends Request {
    lockId?: string;
}

export default RequestWithLockId;